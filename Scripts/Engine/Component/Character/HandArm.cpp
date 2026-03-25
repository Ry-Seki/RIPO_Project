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
#include "../../Audio/AudioUtility.h"
#include "../../Stage/StageObject/Treasure/Treasure.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Audio/LoadAudio.h"
#include "../../Load/Model/LoadModel.h"
#include "../../Component/StrengthComponent.h"

using namespace StageObjectUtility;
using namespace CharacterUtility;
using namespace AudioUtility;

HandArm::HandArm()
	: liftObject(nullptr)
	, playerModelHandle(-1)
	
	, LIFT_POS_RATE(300)
	, LIFTABLE_DISTANCE(1000)
	, LIFT_HEIGHT(200)
{}

void HandArm::Start() {
	// ロード
	auto playerModel = LoadManager::GetInstance().LoadResource<LoadModel>("Res/Model/Player/RIPO_Model.mv1");
	auto liftSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/PlayerSE/Lift.mp3");
	auto notStrengthSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/PlayerSE/NotStrength.mp3");
	LoadManager::GetInstance().SetOnComplete([this, playerModel, liftSE, notStrengthSE]() {
		SetModelHandle(playerModel->GetHandle());
		RegisterSEHandle(GameConst::_PLAYER_LIFT_SE, liftSE->GetHandle());
		RegisterSEHandle(GameConst::_PLAYER_NOT_STRENGTH_SE, notStrengthSE->GetHandle());
	});
}

void HandArm::ArmUpdate(float deltaTime, ActionMapBase::ActionState action, Engine* engine) {
	GameObjectPtr player = GetPlayer();
	// お宝持ち上げ
	int lift = static_cast<int>(GameEnum::PlayerAction::Lift);
	if (action.buttonDown[lift])
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
			return distance < LIFTABLE_DISTANCE &&
				col &&
				col->GetOwner()->name != GameConst::_CREATE_POSNAME_PLAYER &&
				col->GetOwner()->name == GameConst::_CREATE_POSNAME_TREASURE;
		}
	);

	// お宝がヒットしたら必要ストレングスによって持つ
	if (!hit)
		return;
	if (auto treasure = hitInfo.collider->GetOwner()->GetComponent<Treasure>()) {
		int playerStrength = player->GetComponent<StrengthComponent>()->GetStrength();
		if (liftObject)
			return;
		if (treasure->GetStrength() > playerStrength) {
			// SE再生
			PlaySE(GameConst::_PLAYER_NOT_STRENGTH_SE);
			return;
		}

		liftObject = hitInfo.collider->GetOwner();
		// SE再生
		PlaySE(GameConst::_PLAYER_LIFT_SE);
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
	liftObject->position = player->position + (ForwardDir(cameraRot) * LIFT_POS_RATE);
	liftObject->position.y += LIFT_HEIGHT;
}

/*
 *	モデルセット
 */
void HandArm::SetModelHandle(int setModelHandle) {
	playerModelHandle = setModelHandle;
}
