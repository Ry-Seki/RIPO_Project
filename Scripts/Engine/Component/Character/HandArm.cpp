/*
 *	@file	HandArm.cpp
 *	@author Riku
 */

#include "HandArm.h"
#include "../../Manager/CameraManager.h"
#include "../../VecMath.h"
#include "../../GameConst.h"
#include "../../Scene/Scene.h"
#include "../../Stage/StageObject/StageObjectUtility.h"
#include "../../Component/Character/CharacterUtility.h"

using namespace StageObjectUtility;
using namespace CharacterUtility;

#include "../../GameObject/GameObjectUtility.h"
using namespace GameObjectUtility;
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
	int lift = static_cast<int>(GameEnum::PlayerAction::Lift);
	if (action.button[lift])
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
	Scene::RayCastHit hitInfo;
	bool hit = engine->GetCurrentScene()->RayCast(
		ray, hitInfo,
		[this](const ColliderBasePtr& col, float distance) {
			// 交点が指定値以内かつプレイヤー以外の宝オブジェクト
			return distance < LEFTABLE_DISTANCE &&
				col &&
				col->GetOwner()->name != GameConst::_CREATE_POSNAME_PLAYER &&
				col->GetOwner()->name == GameConst::_CREATE_POSNAME_TREASURE;
		}
	);
	// 条件内でヒットすればそのオブジェクトを保存
	if (hit) {
		// 仮でパワーを反映する(IDで宝を判定して無理やり制限を掛ける)
		auto player = GetPlayer()->GetComponent<PlayerComponent>();
		int ID1 = -1;
		int ID2 = -1;
		auto objectList = GetCreateObjectList();
		for (auto& obj : objectList) {
			if (obj->name == GameConst::_CREATE_POSNAME_TREASURE) {
				if (ID1 == -1) {
					ID1 = obj->ID;
					continue;
				}
				else if (ID2 == -1) {
					ID2 = obj->ID;
					break;
				}
			}
		}
		if (hitInfo.collider->GetOwner()->ID == ID1) {
			liftObject = hitInfo.collider->GetOwner();
		}
		else if (hitInfo.collider->GetOwner()->ID == ID2) {
			if (player->GetPlayerStatus().strength > 2) {
				liftObject = hitInfo.collider->GetOwner();
			}
		}
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
