/*
 *	@file	AmmoCountUI.cpp
 *	@author	Riku
 */

#include "AmmoCountUI.h"
#include "../../Manager/WeaponManager.h"
#include "../../GameConst.h"

/*
 *	•`‰æˆ—
 */
void AmmoCountUI::Render() {
	auto weapon = WeaponManager::GetInstance().GetCurrentWeapon();
	float posX = GameConst::WINDOW_WIDTH * 0.9f;
	float posY = GameConst::WINDOW_HEIGHT * 0.9f;
	DrawFormatString(posX, posY, GetColor(255, 255, 255), "%d / %d", weapon->ammoCount, weapon->ammoCountMax);
}
