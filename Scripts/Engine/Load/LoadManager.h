/*
 *	@file	LoadManager.h
 *	@author	Seki
 */

#ifndef _LOAD_MANAGER_H_
#define _LOAD_MANAGER_H_

#include "../Singleton.h"
#include "LoadSystem.h"
#include "LoadRegistry.h"

#include <functional>
#include <memory>
#include <queue>
#include <mutex>

 /*
  *	ファイル読み込み管理クラス（多段階ロード対応版）
  */
class LoadManager : public Singleton<LoadManager> {
    // フレンド宣言
    friend class Singleton<LoadManager>;

private:
    std::unique_ptr<LoadSystem> system;                     // ロードの内部処理(常に持っている想定)
    std::queue<std::function<void()>> onCompleteQueue;      // コールバックキュー
    LoadRegistry loadRegistry;                              // リソースキャッシュ

    bool lastLoadWasCached = false;
    std::mutex managerMutex;

private:
    /*
     *  コンストラクタ
     */
    LoadManager() : system(std::make_unique<LoadSystem>()) {}
    /*
     *  デストラクタ
     */
    ~LoadManager() = default;

public:
    /*
     *  更新処理
     */
    void Update(float unscaledDeltaTime) {
        if (!system->IsComplete()) {
            system->Update(unscaledDeltaTime);
        }
        // ロード完了判定
        if (system->IsComplete() && !onCompleteQueue.empty()) {
            auto callback = onCompleteQueue.front();
            onCompleteQueue.pop();
            if (callback) callback();

            // 完了後、新しいロードが入るかもしれないため isComplete を false に戻す
            system->ResetCompleteFlag();
        }
    }
    /*
     *  描画処理
     */
    void Render() {
        if (system) system->Render();
    }

public:
    /*
     *  リソースの読み込み(コールバックの設定込み)
     *  @param[in]  const std::string& setFilePath      ファイルパス
     *  @return     std::shared_ptr<T> TはLoadBasePtrの派生クラス
     */
    template<class T, typename = std::enable_if_t<std::is_base_of_v<LoadBase, T>>>
    std::shared_ptr<T> LoadResource(const std::string& setFilePath) {
        if (setFilePath.empty()) return nullptr;
        // registryから取得
        auto cached = loadRegistry.Get(setFilePath);
        if (cached) {
            lastLoadWasCached = true;
            auto resource = std::dynamic_pointer_cast<T>(cached);  
            return resource;
        }
        lastLoadWasCached = false;
        auto loader = std::make_shared<T>(setFilePath);
        loadRegistry.Register(setFilePath, loader);
        system->AddLoader(loader);

        return loader;
    }
    /*
     *  コールバック登録
     */
    void SetOnComplete(const std::function<void()>& callback) {
        if (!callback) return;

        // ミューテックスで排他
        std::lock_guard<std::mutex> lock(managerMutex);

        // すでにロードが全完了状態なら、即発火（新しいコールバックのみ）
        if (lastLoadWasCached) {
            callback();
            return;
        }

        // そうでない場合は通常通りキューに積む
        onCompleteQueue.push(callback);
    }
    /*
     *  ロードアニメーションに追加
     */
    void AddAnimation(const LoadAnimationPtr& animation) {
        system->AddAnimation(animation);
    }
    /*
     *  ロード内容をクリア
     *  （コールバックキューは保持したまま）
     */
    void Clear(bool clearCallbacks = false) {
        system->Clear();
        if (clearCallbacks) {
            while (!onCompleteQueue.empty()) onCompleteQueue.pop();
        }
    }
public:
    /*
     *  ロード中判定
     */
    inline bool IsLoading() const {
        return !system->IsComplete();
    }
    /*
     *  進捗度取得
     */
    inline float GetProgress() const {
        return system ? system->GetProgress() : 1.0f;
    }
};

#endif // !_LOAD_MANAGER_H_