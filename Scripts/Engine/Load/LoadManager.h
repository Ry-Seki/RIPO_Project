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

/*
 *	@brief  ファイル読み込み管理クラス
 */
class LoadManager : public Singleton<LoadManager> {
    // フレンド宣言
    friend class Singleton<LoadManager>;

private:
    LoadSystem system;                                      // ロードの内部処理
    std::queue<std::function<void()>> onCompleteQueue;      // コールバックキュー
    LoadRegistry loadRegistry;                              // リソースキャッシュ

private:
    /*
     *  @brief  コンストラクタ
     */
    LoadManager() = default;
    /*
     *  @brief  デストラクタ
     */
    ~LoadManager() = default;

public:
    /*
     *  @brief  更新処理
     */
    void Update(float unscaledDeltaTime) {
        // ロードシステムの更新
        system.Update(unscaledDeltaTime);
        // コールバックの使用
        ActiveCallback();
    }
    /*
     *  @brief  描画処理
     */
    void Render() {
        system.Render();
    }
    /*
     *  @brief  解放処理
     */
    void Clear() {
        system.Teardown();
        loadRegistry.Clear();
    }

public:
    /*
     *  @brief      リソースの読み込み(コールバックの設定込み)
     *  @param[in]  const std::string& setFilePath      ファイルパス
     *  @return     std::shared_ptr<T> TはLoadBasePtrの派生クラス
     */
    template<class T, typename = std::enable_if_t<std::is_base_of_v<LoadBase, T>>>
    std::shared_ptr<T> LoadResource(const std::string& setFilePath) {
        if (setFilePath.empty()) return nullptr;
        // registryから取得
        auto cached = loadRegistry.Get(setFilePath);
        if (cached) {
            auto resource = std::dynamic_pointer_cast<T>(cached);
            return resource;
        }
        auto loader = std::make_shared<T>(setFilePath);
        loadRegistry.Register(setFilePath, loader);
        system.AddLoader(loader);
        system.ResetCompleteFlag();

        return loader;
    }
    /*
     *  @brief  コールバック登録
     */
    void SetOnComplete(const std::function<void()>& callback) {
        if (!callback) return;

        // そうでない場合は通常通りキューに積む
        onCompleteQueue.push(callback);
    }
    /*
     *  @brief  コールバックの実行
     */
    void ActiveCallback() {
        while (!onCompleteQueue.empty() && system.IsComplete()) {
            std::function<void()> callback;
            if (onCompleteQueue.empty()) break;
            callback = onCompleteQueue.front();
            onCompleteQueue.pop();
            
            if (callback) callback();
        }
        // キューが空なら自動でロードシステムをリセット
        if (onCompleteQueue.empty() && system.IsComplete()) {
            system.Clear();
        }
    }

public:
    /*
     *  @brief      ロード中判定
     *  @return     bool
     */
    inline bool IsLoading() const {
        return !system.IsComplete();
    }
    /*
     *  @brief  ロードアニメーションに追加
     */
    inline void AddAnimation(const LoadAnimationPtr& animation) {
        system.AddAnimation(animation);
    }
};

#endif // !_LOAD_MANAGER_H_