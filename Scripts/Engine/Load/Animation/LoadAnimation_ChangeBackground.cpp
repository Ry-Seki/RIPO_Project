/*
 *	@file	LoadAnimation_Background.cpp
 *	@author	Seki
 */

#include "LoadAnimation_ChangeBackground.h"

/*
 *	更新処理
 */
void LoadAnimation_ChangeBackground::Update(float unscaledDeltaTime) {
    if (backgroundList.empty()) return;

    elapsedTime += unscaledDeltaTime;

    BGImage& currentImage = backgroundList[currentIndex];
    BGImage& nextImage = backgroundList[(currentIndex + 1) % backgroundList.size()];

    // 一定時間経過でフェード開始
    if (elapsedTime > switchInterval) {
        fadingOut = true;
        elapsedTime = 0.0f;
    }

    // フェード処理
    if (fadingOut) {
        currentImage.alpha -= (255.0f / fadeTime) * unscaledDeltaTime;
        nextImage.alpha += (255.0f / fadeTime) * unscaledDeltaTime;

        if (currentImage.alpha <= 0.0f) {
            currentImage.alpha = 0.0f;
            nextImage.alpha = 255.0f;
            currentIndex = (currentIndex + 1) % backgroundList.size();
            fadingOut = false;
        }
    }
}
/*
 *  描画処理
 */
void LoadAnimation_ChangeBackground::Render() {
    if (backgroundList.empty()) return;

    // 背景の描画
    for (auto& background : backgroundList) {
        if (background.alpha > 0) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)background.alpha);
            DrawGraph(0, 0, background.spriteHandle, FALSE);
        }
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
/*
 *  解放処理
 */
void LoadAnimation_ChangeBackground::Unload() {
    for (auto& bg : backgroundList) {
        if (bg.spriteHandle != -1) {
            DeleteGraph(bg.spriteHandle);
            bg.spriteHandle = -1;
        }
    }
    backgroundList.clear();
}