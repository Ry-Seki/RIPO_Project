/*
 *	@file	ReticleUI.cpp
 *	@author	Riku
 */

#include "ReticleUI.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../GameConst.h"
#include "AmmoCountUI.h"
#include "../../Manager/WeaponManager.h"
#include "../../Manager/CameraManager.h"

void ReticleUI::Initialize() {
	LoadManager& load = LoadManager::GetInstance();
	auto reticleSprite = load.LoadResource<LoadSprite>(RETICLE_PATH);
	load.SetOnComplete([this, reticleSprite]() {
		SetGraphHandle(reticleSprite->GetHandle());
		});
}

void ReticleUI::Render() {
	// カメラが3人称なら表示なし
	if (CameraManager::GetInstance().GetCameraState() == GameEnum::CameraState::TPS)
		return;
	auto weapon = WeaponManager::GetInstance().GetCurrentWeapon();
	if (!weapon)
		return;
	// リロード中は表示なし
	if (weapon->reload)
		return;
	// 画面中心に描画できるように計算
	float centerWidth = GameConst::WINDOW_WIDTH * 0.5f;
	float centerHeight = GameConst::WINDOW_HEIGHT * 0.5f;
	int graphSizeWidth;
	int graphSizeHeight;
	if (!reticleGraphHandle) return;
	// 画面中心座標から画像のサイズを引いた値が最終座標
	GetGraphSize(reticleGraphHandle, &graphSizeWidth, &graphSizeHeight);
	float drawPosX = centerWidth - graphSizeWidth * 0.5f;
	float drawPosY = centerHeight - graphSizeHeight * 0.5f;
	
	// 描画
	DrawGraph(drawPosX, drawPosY, reticleGraphHandle, TRUE);
}

/*
 *	グラフハンドルのセット
 */
void ReticleUI::SetGraphHandle(int setHandle) {
	reticleGraphHandle = setHandle;
}
