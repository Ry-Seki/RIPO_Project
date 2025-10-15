/*
 *	@file	LoadManager.h
 *	@author	Seki
 */

#ifndef _LOAD_MANAGER_H_
#define _LOAD_MANAGER_H_

#include "../Singleton.h"
#include "LoadSystem.h"
#include <functional>
#include <memory>

/*
 *	ファイル読み込み管理クラス
 */
class LoadManager : public Singleton<LoadManager> {
private:
    std::shared_ptr<LoadSystem> system;
    std::function<void()> onComplete;

public:
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
        if (!system || system->IsComplete()) return;

        system->Update(unscaledDeltaTime);

        if (system->IsComplete() && onComplete) {
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
     *  コールバック処理設定
     *  param[in]   const std::function<void()>& callback   void型のコールバック
     */
    void SetOnComplete(const std::function<void()>& callback) { onComplete = callback; }
    /*
     *  ロード対象追加
     *  param[in]   const LoadBasePtr& loader   ロード対象
     */
    void AddLoader(const LoadBasePtr& loader) { system->AddLoader(loader); }
    /*
     *  アニメーション追加
     *  param[in]   const LoadAnimationPtr& animation    ロードアニメーション
     */
    void AddAnimation(const LoadAnimationPtr& animation) { system->AddAnimation(animation); }
    /*
     *  解放処理
     */
    void Clear() {
        if (system) system->Clear();
        onComplete = nullptr;
    }

public:
    /*
     *  ロード中フラグの取得
     */
    bool IsLoading() const { return system && !system->IsComplete(); }
    /*
     *  進捗度ゲージ追加
     */
    float GetProgress() const { return system ? system->GetProgress() : 1.0f; }
};

#endif // !_LOAD_MANAGER_H_