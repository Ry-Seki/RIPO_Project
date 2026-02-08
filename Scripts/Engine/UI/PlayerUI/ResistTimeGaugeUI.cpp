/*
 *	@file	ResistTimeGaugeUI.cpp
 *	@author	Riku
 */

#include "ResistTimeGaugeUI.h"
#include "../../Manager/WeaponManager.h"
#include "../../GameConst.h"

 /*
  *	•`‰æˆ—
  */
void ResistTimeGaugeUI::Render() {
	// •`‰æˆÊ’uŒvŽZ
	auto weapon = WeaponManager::GetInstance().GetCurrentWeapon();
	float posSX = GameConst::WINDOW_WIDTH * 0.2f;
	float posSY = GameConst::WINDOW_HEIGHT * 0.9f;
	float changeValue = 0.8f;
	if (weapon->reloadingTime > 0)
		changeValue = (weapon->reloadingTime / weapon->reloadingTimeMax) * 0.8f;
	float posEX = GameConst::WINDOW_WIDTH * changeValue;
	float posEY = GameConst::WINDOW_HEIGHT * 0.95f;
	// •`‰æ
	DrawBox(posSX, posSY, posEX, posEY, GetColor(80, 255, 20), TRUE);
}
