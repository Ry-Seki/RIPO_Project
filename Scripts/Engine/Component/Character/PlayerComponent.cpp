/*
 *	@file	PlayerComponent.cpp
 *  @author	Riku
 */

#include "PlayerComponent.h"
#include "../../Manager/CameraManager.h"
#include "DxLib.h"

PlayerComponent::PlayerComponent() 
	: moveSpeed(5.0f)
	, DEFAULT_MOVE_SPEED(5.0f)
	, acceleration(2.0f){
}

void PlayerComponent::Update(float deltaTime) {
	PlayerMove();
	PlayerRun();
}

void PlayerComponent::PlayerMove() {
	GameObject* player = GetOwner();
	GameObjectPtr camera = CameraManager::GetInstance().GetCamera();

	// カメラの角度
	const float cameraCos = cosf(camera->rotation.y);
	const float cameraSin = sinf(camera->rotation.y);

	// カメラから見たX軸移動
	if (CheckHitKey(KEY_INPUT_A)) {
		player->position.x -= moveSpeed * cameraCos;
		player->position.z -= moveSpeed * -cameraSin;
	}
	if (CheckHitKey(KEY_INPUT_D)) {
		player->position.x += moveSpeed * cameraCos;
		player->position.z += moveSpeed * -cameraSin;
	}
	// カメラから見たZ軸移動
	if (CheckHitKey(KEY_INPUT_S)) {
		player->position.x -= moveSpeed * cameraSin;
		player->position.z -= moveSpeed * cameraCos;
	}
	if (CheckHitKey(KEY_INPUT_W)) {
		player->position.x += moveSpeed * cameraSin;
		player->position.z += moveSpeed * cameraCos;
	}
	// デバッグ用Y軸移動
	if (CheckHitKey(KEY_INPUT_LCONTROL)) {
		player->position.y -= moveSpeed;
	}
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		player->position.y += moveSpeed;
	}
}

void PlayerComponent::PlayerRun() {
	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		moveSpeed = DEFAULT_MOVE_SPEED * acceleration;
	}
	else {
		moveSpeed = DEFAULT_MOVE_SPEED;
	}
}
