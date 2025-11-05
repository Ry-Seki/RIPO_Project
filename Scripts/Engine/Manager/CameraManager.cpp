/*
 *	@file	CameraManager.cpp
 *  @author	Riku
 */

#include "CameraManager.h"
#include "GameObjectManager.h"

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
	GameObjectManager::GetInstance().ResetObject(camera);
	camera = nullptr;
}
