/*
 *	@file	HandArm.cpp
 *	@author Riku
 */

#include "HandArm.h"
#include "../../Manager/CameraManager.h"
#include "../../VecMath.h"
#include "../../GameConst.h"
#include "../../GameEnum.h"
#include "../../Scene/Scene.h"
#include "../../Stage/StageObject/StageObjectUtility.h"
#include "../../Component/Character/CharacterUtility.h"
#include "../../Stage/StageObject/Treasure/Treasure.h"

using namespace StageObjectUtility;
using namespace CharacterUtility;

HandArm::HandArm()
	: liftObject(nullptr)
	
	, LEFTABLE_DISTANCE(1000)
{}

void HandArm::Start() {
	playerModelHandle = MV1LoadModel("Res/Model/Player/RIPO_Model.mv1");
}

void HandArm::ArmUpdate(float deltaTime, ActionMapBase::ActionState action, Engine* engine) {
	GameObjectPtr player = GetPlayer();
	// 右クリックでお宝持ち上げ
	int lift = static_cast<int>(GameEnum::PlayerAction::Lift);
	if (action.button[lift])
		LiftTreasure(player, engine);
	CarryTreasure(player);
}

/*
 *	お宝持ち上げ処理
 */
void HandArm::LiftTreasure(GameObjectPtr player, Engine* engine) {
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

	// お宝がヒットしたら必要ストレングスによって持つ
	if (!hit)
		return;
	if (auto treasure = hitInfo.collider->GetOwner()->GetComponent<Treasure>()) {
		int playerStrength = player->GetComponent<PlayerComponent>()->GetPlayerStatus().strength;
		if (treasure->GetStrength() > playerStrength || liftObject)
			return;

		liftObject = hitInfo.collider->GetOwner();
		// 視点変更イベント再生
		CameraManager::GetInstance().CameraEventPlay(GameEnum::CameraEvent::ChangeView);
		// プレイヤーの描画モデル変更
		SetCharacterModel(player.get(), playerModelHandle);
	}
}

/*
 *	お宝運び処理
 */
void HandArm::CarryTreasure(GameObjectPtr player) {
	if (liftObject == nullptr) return;

	GameObjectPtr camera = CameraManager::GetInstance().GetCamera();
	auto cameraRot = camera->rotation;
	cameraRot.x = 0;
	liftObject->position = player->position + (ForwardDir(cameraRot) * 500);
}
