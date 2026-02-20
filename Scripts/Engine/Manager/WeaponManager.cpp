/*
 *	@file	WeaponManager.cpp
 *	@author	Riku
 */

#include "WeaponManager.h"
#include "../Component/Character/RevolverArm.h"
#include "../Component/Character/SubmachineGunArm.h"

WeaponManager::WeaponManager() 
	: submachineGun(false)
{}

/*
 *　初期化
 */
void WeaponManager::Initialize() {
	weapons[GameEnum::Weapon::Revolver] = std::make_shared<RevolverArm>();
	weapons[GameEnum::Weapon::Revolver]->WeaponStart();
	weapons[GameEnum::Weapon::SubmachineGun] = std::make_shared<SubmachineGunArm>();
	weapons[GameEnum::Weapon::SubmachineGun]->WeaponStart();
	// 初期設定はリボルバー
	currentWeapon = weapons[GameEnum::Weapon::Revolver];
	
}

/*
 *	使用する武器の設定
 */
void WeaponManager::SetCurrentWeapon(GameEnum::Weapon setWeapon) {
	if (currentWeapon == weapons[setWeapon])
		return;
	// サブマシンガンが解放されていないならスキップ(修正予定)
	if (setWeapon == GameEnum::Weapon::SubmachineGun && !submachineGun)
		return;
	currentWeapon = weapons[setWeapon];
}

/*
 *	使用中武器の取得
 */
WeaponBasePtr WeaponManager::GetCurrentWeapon() {
	if (!currentWeapon) return nullptr;
	return currentWeapon;
}

/*
 *　使用中武器の番号取得
 */
GameEnum::Weapon WeaponManager::GetCurrentWeaponNum() {
	return currentWeapon->GetNumber();
}

/*
 *	サブマシンガン解放
 */
void WeaponManager::UnlockSubmachineGun() {
	submachineGun = true;
}

/*
 *	サブマシンガンが解放されているか
 */
bool WeaponManager::IsSubmachineGun() {
	return submachineGun;
}
/*
 *	サブマシンガン解放フラグの設定
 */
void WeaponManager::SetIsSubmachinGun(bool setFlag) {
	submachineGun = setFlag;
}
