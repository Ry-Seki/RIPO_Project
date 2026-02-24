/*
 *	@file	ResistTimeGaugeUI.cpp
 *	@author	Riku
 */

#include "ResistTimeGaugeUI.h"
#include "../../Component/Character/CharacterUtility.h"
#include "../../System/Status/PlayerStatusManager.h"
#include "../../GameConst.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"

using namespace CharacterUtility;

/*
 *	初期化処理
 */
void ResistTimeGaugeUI::Initialize() {
	LoadManager& load = LoadManager::GetInstance();
	auto resistSprite = load.LoadResource<LoadSprite>(RESIST_PATH);
	load.SetOnComplete([this, resistSprite]() {
		SetGraphHandle(resistSprite->GetHandle());
		});
}

/*
 *	描画処理
 */
void ResistTimeGaugeUI::Render() {
	// 描画位置計算
	float playerResist = static_cast<float>(GetPlayer()->GetComponent<PlayerComponent>()->GetPlayerStatus().resistTime);
	float baseResist = static_cast<float>(PlayerStatusManager::GetInstance().GetPlayerStatusData().base.resistTime);
	float posSX = GameConst::WINDOW_WIDTH * 0.2f;
	float posSY = GameConst::WINDOW_HEIGHT * 0.93f;
	float changeValue = 0.2f;
	if (playerResist > 0)
		changeValue = 0.2f + (playerResist / baseResist) * 0.6f;
	float posEX = GameConst::WINDOW_WIDTH * changeValue;
	float posEY = GameConst::WINDOW_HEIGHT * 0.95f;
	// 描画
	DrawBox(posSX, posSY, posEX, posEY, GetColor(213, 255, 147), TRUE);
	float graphX = GameConst::WINDOW_WIDTH * 0.1f;
	float graphY = GameConst::WINDOW_HEIGHT * 0.9f;
	DrawGraph(graphX, graphY, resistTimeGraphHandle, TRUE);
}

/*
 *	グラフハンドルのセット
 */
void ResistTimeGaugeUI::SetGraphHandle(int setHandle) {
	resistTimeGraphHandle = setHandle;
}
