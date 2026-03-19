/*
 *  @file   FadeManager.h
 *  @author Seki
 */

#ifndef _FADE_MANAGER_H_
#define _FADE_MANAGER_H_

#include "FadeBase.h"
#include "../Singleton.h"

#include <functional>
#include <queue>

/*
 *  @brief  フェード管理処理
 */
class FadeManager : public Singleton<FadeManager>{
    // フレンド宣言
    friend class Singleton<FadeManager>;
private:
    FadeBasePtr currentFade;
    std::function<void()> FadeCallback; // フェードのコールバック

private:
    /*
     *  コンストラクタ
     */
    FadeManager() = default;
    /*
     *  デストラクタ
     */
    ~FadeManager() = default;

public:
    /*
     *  フェード開始処理
     */
    void StartFade(const std::shared_ptr<FadeBase>& setFade, std::function<void()> callback = nullptr) {
        // すでにフェード中ならコールバックの実行
        if (currentFade && FadeCallback) {
            auto prevCallback = FadeCallback;
            FadeCallback = nullptr;
            prevCallback();
        }
        currentFade = setFade;
        FadeCallback = callback;
    }
    /*
     *  更新処理
     */
    void Update(float unscaledDeltaTime) {
        if (!currentFade) return;

        // フェードの更新処理の呼び出し
        currentFade->Update(unscaledDeltaTime);

        // フェード終了時
        if (currentFade->IsFinished()) {
            auto callback = FadeCallback; 
            currentFade = nullptr;
            FadeCallback = nullptr;

            // コールバック処理
            if (callback) callback();
        }
    }
    /*
     *  描画処理
     */
    void Render() {
        if (currentFade) currentFade->Render();
    }

public:
    /*
     *  フェードモード取得
     *  @return     FadeMode
     */
    FadeMode GetMode() const {
        return currentFade ? currentFade->GetMode() : FadeMode::NonStop;
    }
    /*
     *  フェード中かどうか
     *  @return     bool
     */
    bool IsFading() const {
        return currentFade != nullptr;
    }
}; 
#endif // !_FADE_MANAGER_H_