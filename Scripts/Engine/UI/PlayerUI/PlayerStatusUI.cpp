/*
 *	@file	PlayerStatusUI.cpp
 *	@author Riku
 */

#include "PlayerStatusUI.h"
#include "../../Component/Character/CharacterUtility.h"
#include "../../System/Status/PlayerStatusManager.h"
#include "../../GameConst.h"

using namespace CharacterUtility;

/*
 *	初期化処理
 */
void PlayerStatusUI::Initialize() {}

/*
 *	描画処理
 */
void PlayerStatusUI::Render() {
	auto playerStatus = GetPlayer()->GetComponent<PlayerComponent>()->GetPlayerStatus();
	auto baseStatus = PlayerStatusManager::GetInstance().GetPlayerStatusData()->base;
	float posX = GameConst::WINDOW_WIDTH * 0.05f;
	float posY = GameConst::WINDOW_HEIGHT * 0.05f;
	DrawFormatString(posX, posY, GetColor(0, 255, 0), "%d / %d", playerStatus.HP, baseStatus.HP);
	posX = GameConst::WINDOW_WIDTH * 0.05f;
	posY = GameConst::WINDOW_HEIGHT * 0.1f;
	DrawFormatString(posX, posY, GetColor(255, 255, 0), "%d / %d", playerStatus.stamina, baseStatus.stamina);
	posX = GameConst::WINDOW_WIDTH * 0.05f;
	posY = GameConst::WINDOW_HEIGHT * 0.15f;
	DrawFormatString(posX, posY, GetColor(255, 0, 0), "%d / %d", playerStatus.strength, baseStatus.strength);

}

/*
 *	グラフハンドルのセット
 */
void PlayerStatusUI::SetGraphHandle(int setHPHandle, int setStaminaHandle, int setStrengthHandle) {}
