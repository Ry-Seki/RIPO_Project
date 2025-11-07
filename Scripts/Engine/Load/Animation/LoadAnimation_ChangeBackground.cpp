/*
 *	@file	LoadAnimation_Background.cpp
 *	@author	Seki
 */

#include "LoadAnimation_ChangeBackground.h"

/*
 *	XVˆ—
 */
void LoadAnimation_ChangeBackground::Update(float unscaledDeltaTime) {
    if (backgroundList.empty()) return;

    frameCounter++;
    if (frameCounter >= switchFrame) {
        frameCounter = 0;
        currentIndex++;
        if (currentIndex >= (int)backgroundList.size()) {
            currentIndex = 0;
        }

        // Œ»İ‚Ì”wŒi‚¾‚¯•\¦
        for (auto& bg : backgroundList) bg.alpha = 0.0f;
        backgroundList[currentIndex].alpha = 255.0f;
    }
}
/*
 *  •`‰æˆ—
 */
void LoadAnimation_ChangeBackground::Render() {
    if (backgroundList.empty()) return;

    // Œ»İ‚Ì”wŒi‚Ì‚İ•`‰æ
    BGImage& currentBG = backgroundList[currentIndex];
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)currentBG.alpha);
    DrawGraph(0, 0, currentBG.spriteHandle, FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // ƒfƒoƒbƒO•\¦
    DrawFormatString(255, 255, GetColor(255, 255, 255), "Load’†");
    DrawFormatString(255, 300, GetColor(255, 255, 255), "LoadIndex : %d", currentIndex);
    DrawFormatString(255, 330, GetColor(255, 255, 0), "elapsedTime : %.2f / %.2f", elapsedTime, switchInterval);
}
/*
 *  ‰ğ•úˆ—
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