/*
 *	@file	ResistTimeGaugeUI.cpp
 *	@author	Riku
 */

#include "ResistTimeGaugeUI.h"
#include "../../Component/Character/CharacterUtility.h"
#include "../../System/Status/PlayerStatusManager.h"
#include "../../GameConst.h"

using namespace CharacterUtility;

 /*
  *	•`‰æˆ—
  */
void ResistTimeGaugeUI::Render() {
	// •`‰æˆÊ’uŒvZ
	float playerResist = static_cast<float>(GetPlayer()->GetComponent<PlayerComponent>()->GetPlayerStatus().resistTime);
	float baseResist = static_cast<float>(PlayerStatusManager::GetInstance().GetPlayerStatusData()->base.resistTime);
	float posSX = GameConst::WINDOW_WIDTH * 0.2f;
	float posSY = GameConst::WINDOW_HEIGHT * 0.93f;
	float changeValue = 0.8f;
	if (playerResist > 0)
		changeValue = (playerResist / baseResist) * 0.8f;
	float posEX = GameConst::WINDOW_WIDTH * changeValue;
	float posEY = GameConst::WINDOW_HEIGHT * 0.95f;
	// •`‰æ
	DrawBox(posSX, posSY, posEX, posEY, GetColor(213, 255, 147), TRUE);
}
