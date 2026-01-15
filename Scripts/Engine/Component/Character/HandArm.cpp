/*
 *	@file	HandArm.cpp
 *	@author Riku
 */

#include "HandArm.h"
#include "../../Manager/CameraManager.h"
#include "../../VecMath.h"
#include "../../GameConst.h"
#include "../../Scene/Scene.h"

HandArm::HandArm()
	: liftObject(nullptr)
	
	, LEFTABLE_DISTANCE(1000)
{}

void HandArm::OnCollision(
	const std::shared_ptr<Component>& self,
	const std::shared_ptr<Component>& other) {
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
	GameObjectPtr camera = CameraManager::GetInstance().GetCamera();
	// レイキャストで持ち上げ対象があるか探す
	Ray ray = { camera->position, ForwardDir(camera->rotation) };
	Scene::RaycastHit hitInfo;
	bool hit = engine->GetCurrentScene()->Raycast(
		ray, hitInfo,
		[this](const ColliderBasePtr& col, float distance) {
			// 交点が指定値以内かつプレイヤー以外の宝オブジェクト
			auto hitName = col->GetOwner()->name;
			return distance < LEFTABLE_DISTANCE &&
				hitName != GameConst::_CREATE_POSNAME_PLAYER &&
				hitName == GameConst::_CREATE_POSNAME_TREASURE;
		}
	);
	// 条件内でヒットすればそのオブジェクトを保存
	if (hit) {
		liftObject = hitInfo.collider->GetOwner();
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
