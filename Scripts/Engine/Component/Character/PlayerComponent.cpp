/*
 *	@file	PlayerComponent.cpp
 *  @author	Riku
 */

#include "PlayerComponent.h"
#include "../../Manager/CameraManager.h"
#include "DxLib.h"

void PlayerComponent::Update(float deltaTime) {
	GameObject* player = GetOwner();
	GameObjectPtr camera = CameraManager::GetInstance().GetCamera();

	// ƒJƒƒ‰‚ÌˆÚ“®
	const float cameraSpeed = 0.05f;
	const float cameraCos = cosf(camera->rotation.y);
	const float cameraSin = sinf(camera->rotation.y);

	// ƒJƒƒ‰‚ÌXŽ²ˆÚ“®
	if (CheckHitKey(KEY_INPUT_A)) {
		camera->position.x -= cameraSpeed * cameraCos;
		camera->position.z -= cameraSpeed * -cameraSin;
	}
	if (CheckHitKey(KEY_INPUT_D)) {
		camera->position.x += cameraSpeed * cameraCos;
		camera->position.z += cameraSpeed * -cameraSin;
	}

	// ƒJƒƒ‰‚ÌZŽ²ˆÚ“®
	if (CheckHitKey(KEY_INPUT_W)) {
		camera->position.x -= cameraSpeed * cameraSin;
		camera->position.z -= cameraSpeed * cameraCos;
	}
	if (CheckHitKey(KEY_INPUT_S)) {
		camera->position.x += cameraSpeed * cameraSin;
		camera->position.z += cameraSpeed * cameraCos;
	}
}
