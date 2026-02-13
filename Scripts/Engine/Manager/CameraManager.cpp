/*
 *	@file	CameraManager.cpp
 *  @author	Riku
 */

#include "CameraManager.h"
#include "../GameObject/GameObjectUtility.h"
#include "../Input/InputUtility.h"

using namespace GameObjectUtility;
using namespace InputUtility;

CameraManager::CameraManager() 
	: engine(nullptr){
}

/*
 *	初期化処理
 */
void CameraManager::Initialize(Engine& setEngine) {
	engine = &setEngine;
}

/*
 *	カメラ生成
 */
void CameraManager::CreateCamera(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation) {
	// 未使用状態のオブジェクト取得
	camera = GameObjectManager::GetInstance().GetUnuseObject();
	// カメラ生成
	CameraComponentPtr cameraComponent = camera->AddComponent<CameraComponent>();
	// データのセット
	camera->SetObjectData(name, position, rotation);
	engine->AddGameObject(camera);
}
/*
 *	カメラのリセット
 *	@author	Seki
 */
void CameraManager::ResetCamera() {
	ResetObject(camera);
	camera = nullptr;
}

/*
 *	カメライベントの再生
 */
void CameraManager::CameraEventPlay(GameEnum::CameraEvent event) {
	// のちに修正予定
	// プレイヤーの入力を切る
	SetActionMapIsActive(GameEnum::ActionMap::PlayerAction, false);
	// カメラステートをイベントに
	CameraComponentPtr cameraCom =  camera->GetComponent<CameraComponent>();
	cameraCom->SetState(GameEnum::CameraState::Event);
	cameraCom->eventCamera->Initialize(camera.get(), event);
}
