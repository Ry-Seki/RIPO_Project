/*
 *	@file	RevolverArm.cpp
 *  @author Riku
 */

#include "RevolverArm.h"

RevolverArm::RevolverArm()
	: shotCoolTime(0.0f)
	, shotCoolTimeMax(0.0f) {
}

/*
 *	初期化処理
 */
void RevolverArm::Initialize() {
	JSON json = GetData();
	shotCoolTimeMax = json["Revolver"]["ShotCoolTime"];
	reloadingTimeMax = json["Revolver"]["ReloadingTime"];
	ammoCountMax = json["Revolver"]["MagazineCapacity"];
	ammoCount = ammoCountMax;
	shotCoolTime = 0.0f;
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
