/*
 *	@file	RevolverArm.cpp
 *  @author Riku
 */

#include "RevolverArm.h"
#include "../../Manager/CameraManager.h"

RevolverArm::RevolverArm()
	: isShot(true)
	, shotCoolTime(0.0f)

	, SHOT_COOL_TIME_MAX(1.0f) 
{}

void RevolverArm::ArmUpdate(float deltaTime, GameObject* player, Engine* engine) {
	// クールタイム
	if (shotCoolTime < 0) {
		shotCoolTime = 0;
		// 左クリックで射撃
		auto camera = CameraManager::GetInstance().GetCamera();
		if (GetMouseInput() & MOUSE_INPUT_LEFT || isShot) {
			ShotBullet(camera->position, camera->rotation);
			shotCoolTime = SHOT_COOL_TIME_MAX;
			isShot = false;
		}
	}
	else {
		shotCoolTime -= deltaTime;
	}
}
