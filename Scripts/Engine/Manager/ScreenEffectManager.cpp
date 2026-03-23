/*
 *	ScreenEffectManager.cpp
 *	@author Riku
 */

#include "ScreenEffectManager.h"
#include "../GameConst.h"
#include <DxLib.h>

/*
 *	画面効果描画
 */
void ScreenEffectManager::Sc)reenEffectRender() {
	// カウントが0になるまで描画
	if (renderCounter > 0) {
		renderEffect();
		renderCounter -= 1;
	}
}

/*
 *	ダメージフラッシュ
 */
void ScreenEffectManager::DamageFlash() {
	renderMaxCount = 10;
	renderCounter = 10;
	renderEffect = [this]() {
		SetDrawBlendMode(DX_BLENDMODE_ADD, 180 * (renderCounter / renderMaxCount));
		auto r = GameConst::COLOR_RED;
		DrawBox(0, 0, GameConst::WINDOW_WIDTH,
			GameConst::WINDOW_HEIGHT,
			GetColor(r.x, r.y, r.z), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		};
}
