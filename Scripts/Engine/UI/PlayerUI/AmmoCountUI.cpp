/*
 *	@file	AmmoCountUI.cpp
 *	@author	Riku
 */

#include "AmmoCountUI.h"
#include "../../Manager/WeaponManager.h"
#include "../../GameConst.h"
#include "../../Manager/FontManager.h"

/*
 *	‰Šú‰»ˆ—
 */
void AmmoCountUI::Initialize() {
	FontManager::GetInstance().LoadFont(80, "ammo");
}

/*
 *	•`‰æˆ—
 */
void AmmoCountUI::Render() {
	auto weapon = WeaponManager::GetInstance().GetCurrentWeapon();
	if (!weapon)
		return;
	float posX = GameConst::WINDOW_WIDTH * 0.8f;
	float posY = GameConst::WINDOW_HEIGHT * 0.85f;
	std::string ammoCount = std::to_string(weapon->ammoCount);
	if (weapon->ammoCount < 10)
		ammoCount = " " + ammoCount;
	std::string ammoCountMax = std::to_string(weapon->ammoCountMax);
	if (weapon->ammoCountMax < 10)
		ammoCountMax = " " + ammoCountMax;
	FontManager::GetInstance().Draw("ammo", posX, posY, ammoCount + " / " + ammoCountMax, GetColor(255, 255, 255));
}
