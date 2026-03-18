/*
 *	@file	ReloadGaugeUI.cpp
 *	@author	Riku
 */

#include "ReloadGaugeUI.h"
#include "../../Manager/WeaponManager.h"
#include "../../GameConst.h"

ReloadGaugeUI::ReloadGaugeUI() 
	: START_WIDTH_POS_RATIO(0.9f)
	, START_HEIGHT_POS_RATIO(0.8f)
	, END_WIDTH_POS_RATIO(0.95f)
	, END_POS_Y_MOVE_RATE(0.5f)
{}

/*
 *	•`‰æˆ—
 */
void ReloadGaugeUI::Render() {
	// •`‰æˆÊ’uŒvŽZ
	auto weapon = WeaponManager::GetInstance().GetCurrentWeapon();
	if (!weapon)
		return;
	float posSX = GameConst::WINDOW_WIDTH * START_WIDTH_POS_RATIO;
	float posSY = GameConst::WINDOW_HEIGHT * START_HEIGHT_POS_RATIO;
	float posEX = GameConst::WINDOW_WIDTH * END_WIDTH_POS_RATIO;
	// ƒŠƒ[ƒh’†‚ÍƒŠƒ[ƒh‚ÌŒo‰ßŽžŠÔ•ª‚ÌƒQ[ƒWc•‚É‚È‚é
	float changeValue = 1;
	if (weapon->reloadingTime > 0)
		changeValue = 1 - ((weapon->reloadingTime / weapon->reloadingTimeMax) * END_POS_Y_MOVE_RATE);
	float posEY = posSY * changeValue;

	// •`‰æ
	auto p = GameConst::COLOR_PURPLE;
	DrawBox(posSX, posSY, posEX, posEY, GetColor(p.x, p.y, p.z), TRUE);
}
