/*
 *	@file	LoadAnimation_ProgressBackground.cpp
 *	@author	Seki
 */
#include "LoadAnimation_ProgressBackground.h"
#include "../LoadManager.h"
#include <DxLib.h>

/*
 *	çXêVèàóù
 */
void LoadAnimation_ProgressBackground::Update(float unscaledDeltaTime) {
	if (backgroundList.empty()) return;

	elapsedTime += unscaledDeltaTime;

	BGImage& currentImage = backgroundList[currentIndex];
	BGImage& nextImage = backgroundList[(currentIndex + 1) % backgroundList.size()];

	// îwåiêÿë÷
	if (elapsedTime > switchInterval) {
		fadingOut = true;
		elapsedTime = 0.0f;
	}

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
 *	ï`âÊèàóù
 */
void LoadAnimation_ProgressBackground::Render() {
	if (backgroundList.empty()) return;

	// îwåiï`âÊ
	for (auto& bg : backgroundList) {
		if (bg.alpha > 0) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)bg.alpha);
			DrawGraph(0, 0, bg.spriteHandle, FALSE);
		}
	}

	// êiíªÉoÅ[ï`âÊ
	int barWidth = 400;
	int barHeight = 20;
	int barX = 200;
	int barY = 550;

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(barX, barY, barX + barWidth, barY + barHeight, GetColor(100, 100, 100), TRUE); // îwåi
}
/*
 *	âï˙èàóù
 */
void LoadAnimation_ProgressBackground::Unload() {
	for (auto& bg : backgroundList) {
		if (bg.spriteHandle != -1) {
			DeleteGraph(bg.spriteHandle);
			bg.spriteHandle = -1;
		}
	}
	backgroundList.clear();
}