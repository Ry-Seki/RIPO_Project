/*
 *	@file	BulletComponent.cpp
 *  @author	Riku
 */

#include "BulletComponent.h"
#include "CharacterUtility.h"
#include "../../GameObject.h"
#include "../../GameConst.h"
#include "../../GameObject/GameObjectUtility.h"
#include "../../Manager/CameraManager.h"

using namespace GameObjectUtility;
using namespace CharacterUtility;

BulletComponent::BulletComponent()
	: destroyTimeCount(3)
	, bullet(nullptr)

	, SHOT_SPEED(10000) 
{}

void BulletComponent::Start() {
	bullet = GetOwner();
}

void BulletComponent::Update(float deltaTime) {
	if (!bullet) return;
	// 前に進む
	Vector3 forward = ForwardDir(bullet->rotation);
	bullet->position += forward * SHOT_SPEED * deltaTime;

	// 破棄カウントダウン
	destroyTimeCount -= deltaTime;
	if (destroyTimeCount < 0) {
		ResetObject(bullet);
	}
}

void BulletComponent::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	// プレイヤーと弾以外の衝突で破棄
	GameObject* playerOwner = CameraManager::GetInstance().GetTarget().get();
	GameObject* otherOwner = other->GetOwner();
	auto isBullet = otherOwner->GetComponent<BulletComponent>();
	if (otherOwner == playerOwner || isBullet)return;
	ResetObject(bullet);
	// 敵に当たったら敵も破棄
	if (otherOwner->name == GameConst::_CREATE_POSNAME_ENEMY) {
		RemoveCharacter(otherOwner);
	}
}
