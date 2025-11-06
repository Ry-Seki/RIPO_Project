/*
 *	@file	BulletComponent.cpp
 *  @author	Riku
 */

#include "BulletComponent.h"
#include "../../GameObject.h"
#include "../../Manager/GameObjectManager.h"
#include "../../Manager/CameraManager.h"

BulletComponent::BulletComponent() 
	: destroyTimeCount(3)

	, SHOT_SPEED(10000)
{}

void BulletComponent::Update(float deltaTime) {
	GameObject* bullet = GetOwner();

	// 前に進む
	Vector3 forward = ForwardDir(bullet->rotation);
	bullet->position = forward * SHOT_SPEED * deltaTime;
	
	// 破棄カウントダウン
	destroyTimeCount -= deltaTime;
	if (destroyTimeCount < 0) {
		GameObjectPtr destroyBullet = GameObjectManager::GetInstance().GetUseObject(bullet->ID);
		GameObjectManager::GetInstance().ResetObject(destroyBullet);
	}
}

void BulletComponent::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	// プレイヤー以外の衝突で破棄
	GameObjectPtr player = CameraManager::GetInstance().GetTarget();
	if (other == player) return;
	GameObjectPtr destroyBullet = GameObjectManager::GetInstance().GetUseObject(bullet->ID);
	GameObjectManager::GetInstance().ResetObject(destroyBullet);
}
