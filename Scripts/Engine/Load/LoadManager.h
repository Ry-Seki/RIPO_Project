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
 *	ファイル読み込み管理クラス
 */
class LoadManager : public Singleton<LoadManager> {
    // フレンド宣言
    friend class Singleton<LoadManager>;
private:
    std::shared_ptr<LoadSystem> system;             // ロードの内部処理
    std::queue<std::function<void()>> taskQueue;    // フレーム単位で処理するタスク
    std::function<void()> onComplete;               // コールバック
    LoadRegistry loadRegistry;                      // リソース管理

private:
    /*
     *  コンストラクタ
     */
    LoadManager() : system(std::make_shared<LoadSystem>()) {}
    /*
     *  デストラクタ
     */
    ~LoadManager() = default;

public:
    /*
     *  更新処理
     */
    void Update(float unscaledDeltaTime) {
        if (system && !system->IsComplete()) {
            system->Update(unscaledDeltaTime);
        }

        // フレーム単位タスク実行
        if (!taskQueue.empty()) {
            auto task = taskQueue.front();
            task();
            taskQueue.pop();
        }

        // 完了コールバック
        if ((system && system->IsComplete()) && onComplete) {
            onComplete();
            onComplete = nullptr;
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
    void AddLoader(const LoadBasePtr& loader) { system->AddLoader(loader); }
    /*
     *  ロードアニメーションに追加
     */
    void AddAnimation(const LoadAnimationPtr& animation) { system->AddAnimation(animation); }
    /*
     *  ロードの中身をクリアにする
     */
    void Clear() {
        if (system) system->Clear();
        while (!taskQueue.empty()) taskQueue.pop();
        onComplete = nullptr;
    }

public:
    /*
     *  フレーム単位タスクとして登録（逐次ロード向け）
     */
    void StartTask(const std::function<void()>& task) {
        if (task) taskQueue.push(task);
    }
    /*
     *  LoadRegistry を経由してモデルなどを安全にロード
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
     *  コールバックの設定
     */
    void SetOnComplete(const std::function<void()>& callback) { onComplete = callback; }
    /*
     *  ロード中か取得
     */
    bool IsLoading() const { return system && !system->IsComplete(); }
    /*
     *  ロード進捗度の取得
     */
    float GetProgress() const { return system ? system->GetProgress() : 1.0f; }
};

#endif // !_LOAD_MANAGER_H_