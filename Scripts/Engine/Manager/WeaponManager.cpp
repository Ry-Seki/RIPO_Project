/*
 *	@file	WeaponManager.cpp
 *	@author	Riku
 */

#include "WeaponManager.h"
#include "../Component/Character/RevolverArm.h"
#include "../Component/Character/SubmachineGunArm.h"

/*
 *　初期化
 */
void WeaponManager::Initialize() {
	submachineGun = false;
	weapons[GameEnum::Weapon::Revolver] = std::make_shared<RevolverArm>();
	weapons[GameEnum::Weapon::SubmachineGun] = std::make_shared<SubmachineGunArm>();
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
	currentWeapon->Initialize();
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
