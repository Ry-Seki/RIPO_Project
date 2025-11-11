/*
 *	@file	HandArm.cpp
 *	@author Riku
 */

#include "HandArm.h"
#include "../../RayCast.h"
#include "../../Manager/CameraManager.h"
#include "../../VecMath.h"

HandArm::HandArm()
	: liftObject(nullptr)
	
	, LEFTABLE_DISTANCE(100)
{}

void HandArm::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	liftObject = other->GetOwner();
}

void HandArm::ArmUpdate(float deltaTime, GameObject* player, Engine* engine) {
	// 右クリックでお宝持ち上げ
	if (GetMouseInput() & MOUSE_INPUT_RIGHT)
		LiftTreasure(player, engine);
	CarryTreasur(player);
}

/*
 *	お宝持ち上げ処理
 */
void HandArm::LiftTreasure(GameObject* player, Engine* engine) {
	// 正面にオブジェクトがあるか
	float hitLength = 0;
	GameObject* hitObject;
	GameObjectPtr camera = CameraManager::GetInstance().GetCamera();
	if (RayCast(engine, camera->position, camera->rotation, hitLength, hitObject)) {
		// 持ち上げ可能距離なら持ち上げ対象を保存
		if (hitLength < LEFTABLE_DISTANCE) return;
		liftObject = hitObject;
	}
}

/*
 *	お宝運び処理
 */
void HandArm::CarryTreasur(GameObject* player) {
	if (liftObject == nullptr) return;

	GameObjectPtr camera = CameraManager::GetInstance().GetCamera();
	auto cameraRot = camera->rotation;
	cameraRot.x = 0;
	liftObject->position = player->position + (ForwardDir(cameraRot) * 500);
}
