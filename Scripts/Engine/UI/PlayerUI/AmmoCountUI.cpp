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
	// ƒJƒƒ‰‚ª3lÌ‚È‚ç•\Ž¦‚È‚µ
	if (CameraManager::GetInstance().GetCameraState() == GameEnum::CameraState::TPS)
		return;
	auto weapon = WeaponManager::GetInstance().GetCurrentWeapon();
	if (!weapon)
		return;
	// ”Žš‚ª1Œ…‚Ìê‡‚Í‹ó”’‚ð’Ç‰Á‚Å•`‰æ
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
