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
#include "../../Manager/StageManager.h"
#include "../CapsuleCollider.h"

using namespace GameObjectUtility;
using namespace CharacterUtility;

BulletComponent::BulletComponent()
	: destroyTimeCount(0)
	, moveDirection(V_ZERO)
	, bullet(nullptr)
	, hitDamage(0.0f)
	, shotOwner(nullptr)
	, moveSpeed(10000) 
	, MOVE_STEP(20){
}

void BulletComponent::Update(float deltaTime) {
  	if (!bullet) return;
	// 1フレ前の位置を保存
	prevPos = bullet->position;
	
	// すり抜け対策の為刻んで移動
	for (int i = 0; i < MOVE_STEP; i++) {
		// 着弾地点に進む
		bullet->position += moveDirection * (moveSpeed / MOVE_STEP) *deltaTime;

		// ステージとの当たり判定
		auto stageHandle = StageManager::GetInstance().GetCurrentStageHandle();
		auto collider = GetOwner()->GetComponent<CapsuleCollider>();
		VECTOR pos1 = ToVECTOR(collider->capsule.segment.startPoint + bullet->position);
		VECTOR pos2 = ToVECTOR(collider->capsule.segment.endPoint + bullet->position);
		float radius = collider->capsule.radius;
		auto hit = MV1CollCheck_Capsule(stageHandle, -1, pos1, pos2, radius);
		if (hit.HitNum > 0) {
			// エフェクトを出す
			EffectManager::GetInstance().Instantiate("BulletEliminationEffect", bullet->position);
			ResetObject(bullet);
			return;
		}
	}

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
 *	@param	setDestroyTime	弾が消える時間
 */
void BulletComponent::Setup(
	Vector3 setDirection,
	Vector3 setScale, 
	GameObject* setOwner,
	float setDamage,
	float setSpeed,
	float setDestroyTime) {
	moveDirection = setDirection;
	shotOwner = setOwner;
	hitDamage = setDamage;
	moveSpeed = setSpeed;
	bullet = GetOwner();
	bullet->scale = setScale;
	destroyTimeCount = setDestroyTime;
}
