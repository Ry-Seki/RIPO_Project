/*
 *	@file	RevolverArm.cpp
 *  @author Riku
 */

#include "RevolverArm.h"

/*
 *	初期化処理
 */
void RevolverArm::Initialize() {
	number = GameEnum::Weapon::Revolver;
	// リボルバーのデータ取得
	auto data = WeaponDataManager::GetInstance().GetWeaponData(GameEnum::Weapon::Revolver);

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
void RevolverArm::ArmUpdate(float deltaTime, ActionMapBase::ActionState action) {
	// クールタイム
	if (shotCoolTime <= 0) {
		shotCoolTime = 0;
		// 左クリックで射撃
		int shot = static_cast<int>(GameEnum::PlayerAction::Shot);
		if (action.buttonDown[shot]) {
			if (ammoCount > 0) {
				ShotBullet();
				shotCoolTime = shotCoolTimeMax;
				ammoCount -= 1;
			}
			// 弾がなかったらリロード
			else {
				BulletReload();
			}
		}
	}
	else {
		shotCoolTime -= deltaTime;
	}
}
