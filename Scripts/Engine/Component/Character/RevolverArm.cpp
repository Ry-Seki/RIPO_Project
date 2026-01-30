/*
 *	@file	RevolverArm.cpp
 *  @author Riku
 */

#include "RevolverArm.h"

RevolverArm::RevolverArm()
	: shotCoolTime(0.0f)

	, SHOT_COOL_TIME_MAX(0.5f) {
	ammoCountMax = 6;
	reloadingTimeMax = 3.0f;
}

/*
 *	初期化処理
 */
void RevolverArm::Initialize() {
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
				shotCoolTime = SHOT_COOL_TIME_MAX;
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
