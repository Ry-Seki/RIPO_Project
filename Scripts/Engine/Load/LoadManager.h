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
  *	ファイル読み込み管理クラス（多段階ロード対応版）
  */
class LoadManager : public Singleton<LoadManager> {
    // フレンド宣言
    friend class Singleton<LoadManager>;

private:
    std::shared_ptr<LoadSystem> system;                     // ロードの内部処理
    std::queue<std::function<void()>> taskQueue;            // フレーム単位タスク
    std::queue<std::function<void()>> onCompleteQueue;      // コールバックキュー
    LoadRegistry loadRegistry;                              // リソースキャッシュ

private:
    /*
     *  コンストラクタ
     */
    LoadManager() : system(std::make_shared<LoadSystem>()) {}

public:
    ~LoadManager() = default;

public:
    /*
     *  更新処理
     */
    void Update(float unscaledDeltaTime) {
        if (system && !system->IsComplete()) {
            system->Update(unscaledDeltaTime);
        }

        // タスク実行（フレーム単位）
        if (!taskQueue.empty()) {
            auto task = taskQueue.front();
            task();
            taskQueue.pop();
        }

        // ロード完了判定
        if (system && system->IsComplete() && !onCompleteQueue.empty()) {
            auto callback = onCompleteQueue.front();
            onCompleteQueue.pop();
            if (callback) callback();

            // 完了後、新しいロードが入るかもしれないため isComplete を false に戻す
            if (system) system->ResetCompleteFlag();
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
     *  ロードリストに追加
     */
    void AddLoader(const LoadBasePtr& loader) {
        if (system) system->AddLoader(loader);
    }

    /*
     *  ロードアニメーションに追加
     */
    void AddAnimation(const LoadAnimationPtr& animation) {
        if (system) system->AddAnimation(animation);
    }

    /*
     *  ロード内容をクリア
     *  （コールバックキューは保持したまま）
     */
    void Clear(bool clearCallbacks = false) {
        if (system) system->Clear();
        while (!taskQueue.empty()) taskQueue.pop();
        if (clearCallbacks) {
            while (!onCompleteQueue.empty()) onCompleteQueue.pop();
        }
    }

public:
    /*
     *  フレーム単位タスク登録
     */
    void StartTask(const std::function<void()>& task) {
        if (task) taskQueue.push(task);
    }

    /*
     *  リソースをLoadRegistry経由でロード
     */
    template<typename T>
    std::shared_ptr<T> LoadResource(const std::string& path) {
        auto cached = loadRegistry.Get(path);
        if (cached) return std::dynamic_pointer_cast<T>(cached);

        auto resource = std::make_shared<T>(path);
        loadRegistry.Register(path, resource);
        StartTask([resource]() { resource->Load(); });
        return resource;
    }

public:
    /*
     *  コールバック登録（複数同時対応）
     */
    void SetOnComplete(const std::function<void()>& callback) {
        if (callback) onCompleteQueue.push(callback);
    }

    /*
     *  ロード中判定
     */
    bool IsLoading() const {
        return system && !system->IsComplete();
    }

    /*
     *  進捗度取得
     */
    float GetProgress() const {
        return system ? system->GetProgress() : 1.0f;
    }
};

#endif // !_LOAD_MANAGER_H_