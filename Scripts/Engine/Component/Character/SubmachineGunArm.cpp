/*
 *	@file	SubmachineGunArm.cpp
 *	@author Riku
 */

#include "SubmachineGunArm.h"

 /*
  *	初期化処理
  */
void SubmachineGunArm::WeaponStart() {
	number = GameEnum::Weapon::SubmachineGun;
	// サブマシンガンのデータ取得
	auto data = WeaponDataManager::GetInstance().GetWeaponData(number);

	shotCoolTimeMax = data.shotCoolTime;
	shotCoolTime = 0.0f;
	reloadingTimeMax = data.reloadingTime;
	reloadingTime = 0.0f;
	ammoCountMax = data.magazineCapacity;
	ammoCount = ammoCountMax;
}

/*
 *	更新処理
 */
void SubmachineGunArm::WeaponUpdate(float deltaTime, ActionMapBase::ActionState action, Engine* engine) {
	// クールタイム
	if (shotCoolTime <= 0) {
		shotCoolTime = 0;
		// 左クリックで射撃
		int shot = static_cast<int>(GameEnum::PlayerAction::Shot);
		if (action.button[shot] && ammoCount > 0) {
			ShotBullet(engine);
			shotCoolTime = shotCoolTimeMax;
			ammoCount -= 1;
		}
		else if (action.buttonDown[shot]) {
			// 弾がなかったらリロード
			reload = true;
		}
	}
	else {
		shotCoolTime -= deltaTime;
	}
}
