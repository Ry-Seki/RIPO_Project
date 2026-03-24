/*
 *	ScreenEffectManager.cpp
 *	@author Riku
 */

#include "ScreenEffectManager.h"
#include "../GameConst.h"
#include "../Load/LoadManager.h"
#include "../Load/Sprite/LoadSprite.h"
#include <DxLib.h>

ScreenEffectManager::ScreenEffectManager() 
	: renderEffect(nullptr)
	, renderMaxCount(0.0f)
	, renderCounter(0.0f)
	, damageFlashGraphHandle(-1)

	, DAMAGE_FLASH_COUNT(20)
	, DAMAGE_FLASH_PARAM(255)
{}

/*
 *	初期化
 */
void ScreenEffectManager::Initialize() {
	LoadManager& load = LoadManager::GetInstance();
	auto damageFlashSprite = load.LoadResource<LoadSprite>(DAMAGE_FLASH_PATH);
	load.SetOnComplete([this, damageFlashSprite]() {
		SetGraphHandle(damageFlashSprite->GetHandle());
		});
}

/*
 *	画面効果描画
 */
void ScreenEffectManager::ScreenEffectRender() {
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
	renderMaxCount = DAMAGE_FLASH_COUNT;
	renderCounter = DAMAGE_FLASH_COUNT;
	renderEffect = [this]() {
		SetDrawBlendMode(DX_BLENDMODE_ADD, DAMAGE_FLASH_PARAM * (renderCounter / renderMaxCount));
		DrawGraph(0, 0, damageFlashGraphHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		};
}
