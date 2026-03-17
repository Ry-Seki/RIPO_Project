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
	: WIDTH_POS_RATIO(0.8f)
	, HEIGHT_POS_RATIO(0.85f)
{}

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
	float posX = GameConst::WINDOW_WIDTH * WIDTH_POS_RATIO;
	float posY = GameConst::WINDOW_HEIGHT * HEIGHT_POS_RATIO;
	std::string ammoCount = std::to_string(weapon->ammoCount);
	if (weapon->ammoCount < 10)
		ammoCount = " " + ammoCount;
	std::string ammoCountMax = std::to_string(weapon->ammoCountMax);
	if (weapon->ammoCountMax < 10)
		ammoCountMax = " " + ammoCountMax;
	auto w = GameConst::COLOR_WHITE;
	FontManager::GetInstance().Draw("ammo", posX, posY, ammoCount + " / " + ammoCountMax, GetColor(w.x, w.y, w.z));
}
