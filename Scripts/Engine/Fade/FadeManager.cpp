/*
 *  @file   FadeManager.cpp
 *  @author Seki
 */

#include "FadeManager.h"

/*
 *  @brief      フェード開始処理
 *  @param[in]  const FadeBasePtr& setFade
 *  @param[in]  std::function<void()> callback = nullptr
 */
void FadeManager::StartFade(const FadeBasePtr& setFade, std::function<void()> callback) {
    // すでにコールバックがある場合、コールバック実行
    if (currentFade && FadeCallback) {
        auto prevCallback = FadeCallback;
        FadeCallback = nullptr;
        prevCallback();
    }
    currentFade = setFade;
    FadeCallback = callback;
}
/*
 *  @brief      更新処理
 */
void FadeManager::Update(float unscaledDeltaTime) {
    if (!currentFade) return;

    // フェードの更新処理の呼び出し
    currentFade->Update(unscaledDeltaTime);

    // フェード終了時
    if (currentFade->IsFinished()) {
        auto callback = FadeCallback;
        currentFade.reset();
        FadeCallback = nullptr;

        // コールバック処理
        if (callback) callback();
    }
}
/*
 *  @brief      描画処理
 */
void FadeManager::Render() {
    if (currentFade) currentFade->Render();
}
