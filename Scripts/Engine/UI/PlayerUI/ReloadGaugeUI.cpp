/*
 *	@file	ReloadGaugeUI.cpp
 *	@author	Riku
 */

#include "ReloadGaugeUI.h"
#include "../../Manager/WeaponManager.h"
#include "../../GameConst.h"

/*
 *	•`‰æˆ—
 */
void ReloadGaugeUI::Render() {
	// •`‰æˆÊ’uŒvŽZ
	auto weapon = WeaponManager::GetInstance().GetCurrentWeapon();
	float posSX = GameConst::WINDOW_WIDTH * 0.9f;
	float posSY = GameConst::WINDOW_HEIGHT * 0.8f;
	float posEX = GameConst::WINDOW_WIDTH * 0.95f;
	float changeValue = 1;
	if (weapon->reloadingTime > 0)
		changeValue = 1 - ((weapon->reloadingTime / weapon->reloadingTimeMax) * 0.5f);
	float posEY = posSY * changeValue;
	// •`‰æ
	DrawBox(posSX, posSY, posEX, posEY,GetColor(175, 48, 188), TRUE);
}
