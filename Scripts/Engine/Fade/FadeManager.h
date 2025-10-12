/*
 *  @file   FadeManager.h
 *  @author Seki
 */

#ifndef _FADE_MANAGER_H_
#define _FADE_MANAGER_H_

#include "FadeBase.h"
#include "../Singleton.h"

#include <memory>
#include <functional>

/*
 *  フェード管理処理
 */
class FadeManager : public Singleton<FadeManager>{
private:
    FadeBasePtr currentFade;
    std::function<void()> onFadeFinished; // フェード終了時のコールバック

public:
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
        currentFade = setFade;
        onFadeFinished = callback;
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
            auto callback = onFadeFinished; 
            currentFade.reset(); 
            onFadeFinished = nullptr;

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
    /*
     *  フェードモード取得
     */
    FadeMode GetMode() const {
        return currentFade ? currentFade->GetMode() : FadeMode::NonStop;
    }
    /*
     *  フェード中かどうか
     */
    bool IsFading() const {
        return currentFade != nullptr;
    }
}; 
#endif // !_FADE_MANAGER_H_