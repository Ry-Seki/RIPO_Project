/*
 *	@file	BulletComponent.cpp
 *  @author	Riku
 */

#include "BulletComponent.h"
#include "CharacterUtility.h"
#include "../../GameObject.h"
#include "../../GameConst.h"
#include "../../GameEnum.h"
#include "../../GameObject/GameObjectUtility.h"
#include "../../Manager/CameraManager.h"
#include "../../Manager/WeaponManager.h"
#include "../../System/Money/MoneyManager.h"
#include "../../Scene/Scene.h"
#include "../../../Data/WeaponDataManager.h"
#include "../Character/ArmActionComponent.h"
#include "../Character/WeaponBase.h"
#include "../../Manager/EffectManager.h"

using namespace GameObjectUtility;
using namespace CharacterUtility;

BulletComponent::BulletComponent()
	: destroyTimeCount(3)
	, bullet(nullptr)
	, hitDamage(0.0f)

	, moveSpeed(10000) {
}

void BulletComponent::Update(float deltaTime) {
  	if (!bullet) return;
	
	// 着弾地点に進む
	bullet->position += moveDirection * moveSpeed * deltaTime;

	// 破棄カウントダウン
	destroyTimeCount -= deltaTime;
	if (destroyTimeCount < 0) {
		ResetObject(bullet);
	}
}

void BulletComponent::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	// 射撃者と弾以外の衝突で破棄
	GameObject* otherOwner = other->GetOwner();
	auto isBullet = otherOwner->GetComponent<BulletComponent>();
	if (otherOwner == shotOwner || isBullet)return;
	// エフェクトを出す
	EffectManager::GetInstance().Instantiate("BulletEliminationEffect", bullet->position);
	ResetObject(bullet);
}

/*
 *	セットアップ
 *	@param	setDirection	移動方向
 *	@param	setScale		スケール
 *	@param	setOwner		射撃者
 *	@param	setDamage		当たった時に与えるダメージ
 *	@param	setSpeed		弾の速さ
 */
void BulletComponent::Setup(
	Vector3 setDirection,
	Vector3 setScale, 
	GameObject* setOwner,
	float setDamage,
	float setSpeed) {
	moveDirection = setDirection;
	shotOwner = setOwner;
	hitDamage = setDamage;
	moveSpeed = setSpeed;
	bullet = GetOwner();
	bullet->scale = setScale;
}
