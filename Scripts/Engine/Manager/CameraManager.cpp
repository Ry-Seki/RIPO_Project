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
}
