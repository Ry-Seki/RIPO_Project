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
	if (otherOwner == shotOwner.get() || isBullet)return;
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
	GameObjectPtr setOwner,
	float setDamage,
	float setSpeed) {
	moveDirection = setDirection;
	shotOwner = setOwner;
	hitDamage = setDamage;
	moveSpeed = setSpeed;
	bullet->scale = setScale;

	/*
	bullet = GetOwner();
	// 着弾地点を確認
	GameObjectPtr camera = CameraManager::GetInstance().GetCamera();
	Ray ray = { bullet->position, ForwardDir(camera->rotation) };
	Scene::RayCastHit hitInfo;
	bool hit = bullet->GetEngine()->GetCurrentScene()->RayCast(
		ray, hitInfo,
		[this](const ColliderBasePtr& col, float distance) {
			// プレイヤーと自分以外のオブジェクト
			return !col ||
				(col->GetOwner()->name != GameConst::_CREATE_POSNAME_PLAYER &&
					col->GetOwner()->name != "bullet");
		}
	);
	moveDirection = Direction(bullet->position, hitInfo.point);
	// ダメージ設定
	float playerStrength = GetPlayer()->GetComponent<PlayerComponent>()->GetPlayerStatus().strength;
	auto weaponNumber = WeaponManager::GetInstance().GetCurrentWeaponNum();
	float defaultDamage = WeaponDataManager::GetInstance().GetWeaponData(weaponNumber).defaultDamage;
	hitDamage = defaultDamage + (defaultDamage * playerStrength * 0.1f);
	*/

}
