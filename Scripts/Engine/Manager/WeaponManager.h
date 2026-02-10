/*
 *	@file	WeaponManager.h
 *	@author	Riku
 */

#ifndef _WEAPONMANAGAER_H_
#define _WEAPONMANAGAER_H_

#include "../Singleton.h"
#include "../Component/Character/WeaponBase.h"
#include <unordered_map>

/*
 *	武器の管理クラス
 */
class WeaponManager : public Singleton<WeaponManager> {
	friend class Singleton<WeaponManager>;

private:
	WeaponBasePtr currentWeapon;									// 現在設定されている武器
	std::unordered_map<GameEnum::Weapon, WeaponBasePtr> weapons;	// 設定可能な武器達
	bool submachineGun;												// サブマシンガン解放フラグ

private:
	WeaponManager() = default;
	~WeaponManager() = default;

public:
	/* 
	 *　初期化
	 */
	void Initialize();

public:
	/*
	 *	使用する武器の設定
	 */
	void SetCurrentWeapon(GameEnum::Weapon setWeapon);

	/*
	 *	使用中武器の取得
	 */
	WeaponBasePtr GetCurrentWeapon();

	/*
	 *　使用中武器の番号取得
	 */
	GameEnum::Weapon GetCurrentWeaponNum();

	/*
	 *	サブマシンガン解放
	 */
	void UnlockSubmachineGun();
};

#endif // !_WEAPONMANAGAER_H_
