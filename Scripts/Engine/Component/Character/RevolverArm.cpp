/*
 *	@file	RevolverArm.cpp
 *  @author Riku
 */

#include "RevolverArm.h"

RevolverArm::RevolverArm()
	: isShot(true)
	, shotCoolTime(0.0f)

	, SHOT_COOL_TIME_MAX(1.0f) {
	ammoCountMax = 6;
	reloadingTimeMax = 3.0f;
}

void RevolverArm::ArmUpdate(float deltaTime, GameObject* player, Engine* engine) {
	// クールタイム
	if (shotCoolTime < 0) {
		shotCoolTime = 0;
		// 左クリックで射撃
		int shot = static_cast<int>(GameEnum::PlayerAction::Shot);
		if (action.button[shot] || isShot) {
			ShotBullet();
			shotCoolTime = SHOT_COOL_TIME_MAX;
			isShot = false;
		}
	}
	else {
		shotCoolTime -= deltaTime;
	}
}
