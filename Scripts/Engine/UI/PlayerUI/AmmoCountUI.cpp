/*
 *	@file	AmmoCountUI.cpp
 *	@author	Riku
 */

#include "AmmoCountUI.h"
#include "../../Manager/WeaponManager.h"
#include "../../GameConst.h"
#include "../../Manager/FontManager.h"
#include "../../Manager/CameraManager.h"
#include "../../GameEnum.h"

AmmoCountUI::AmmoCountUI() 
	: START_WIDTH_POS_RATIO(0.9f)
	, START_HEIGHT_POS_RATIO(0.8f)
	, END_WIDTH_POS_RATIO(0.95f)
	, END_POS_Y_MOVE_RATE(0.5f)
{}

/*
 *	初期化処理
 */
void AmmoCountUI::Initialize() {
	FontManager::GetInstance().LoadFont(80, "ammo");
}

/*
 *	描画処理
 */
void AmmoCountUI::Render() {
	// カメラが3人称なら表示なし
	if (CameraManager::GetInstance().GetCameraState() == GameEnum::CameraState::TPS)
		return;
	auto weapon = WeaponManager::GetInstance().GetCurrentWeapon();
	if (!weapon)
		return;
	// 描画位置計算
	float posSX = GameConst::WINDOW_WIDTH * START_WIDTH_POS_RATIO;
	float posSY = GameConst::WINDOW_HEIGHT * START_HEIGHT_POS_RATIO;
	float posEX = GameConst::WINDOW_WIDTH * END_WIDTH_POS_RATIO;
	// 残弾数をゲージで表示
	float ammoChangeRate = 1 - ((static_cast<float>(weapon->ammoCount) / static_cast<float>(weapon->ammoCountMax)));
	// リロード中は徐々に上昇
	float reloadChangeRate = 1 - (weapon->reloadingTime / weapon->reloadingTimeMax);
	float changeRate = reloadChangeRate * ammoChangeRate * END_POS_Y_MOVE_RATE;
	float posEY = posSY * END_POS_Y_MOVE_RATE + posSY * changeRate;

	// ゲージ描画
	auto p = GameConst::COLOR_PURPLE;
	DrawBox(posSX, posSY, posEX, posEY, GetColor(p.x, p.y, p.z), TRUE);

	// ゲージ枠描画
	auto w = GameConst::COLOR_WHITE;
	posEY = posSY * END_POS_Y_MOVE_RATE;
	DrawBox(posSX, posSY, posEX, posEY, GetColor(w.x, w.y, w.z), FALSE);

	// ゲージメモリ描画
	for (int i = 1; i < weapon->ammoCountMax; i++) {
		float lineX = posSX + ((posEX - posSX) * 0.5f);
		float lineY = posEY + ((posSY - posEY) * 1 / static_cast<float>(weapon->ammoCountMax) * i);
		DrawLine(posSX, lineY, lineX, lineY, GetColor(w.x, w.y, w.z));
	}
}
