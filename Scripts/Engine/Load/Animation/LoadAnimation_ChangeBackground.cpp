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

    // 一定時間経過したら次の背景へ
    if (elapsedTime >= switchInterval) {
        elapsedTime = 0.0f;
        currentIndex = (currentIndex + 1) % backgroundList.size();

        // 現在の背景だけを表示（他は非表示）
        for (auto& bg : backgroundList) {
            bg.alpha = 0.0f;
        }
        backgroundList[currentIndex].alpha = 255.0f;
    }
}
/*
 *  描画処理
 */
void LoadAnimation_ChangeBackground::Render() {
    if (backgroundList.empty()) return;

    // 現在の背景のみ描画
    BGImage& currentBG = backgroundList[currentIndex];
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)currentBG.alpha);
    DrawGraph(0, 0, currentBG.spriteHandle, FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // デバッグ表示
    DrawFormatString(255, 255, GetColor(255, 255, 255), "Load中");
    DrawFormatString(255, 300, GetColor(255, 255, 255), "LoadIndex : %d", currentIndex);
    DrawFormatString(255, 330, GetColor(255, 255, 0), "elapsedTime : %.2f / %.2f", elapsedTime, switchInterval);
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