/*
 *	@file	BulletManager.cpp
 *  @author	Riku
 */

#include "BulletManager.h"
#include "GameObjectManager.h"
#include "../Component/CapsuleCollider.h"
#include "../Component/ModelRenderer.h"
#include "../Load/LoadManager.h"
#include "../GameConst.h"

#include <DxLib.h>

BulletManager::BulletManager()
	: engine(nullptr)

	, BULLET_CAPSULE_START(V_ZERO)
	, BULLET_CAPSULE_END(V_ZERO)
	, BULLET_CAPSULE_RADIUS(15)
{}

/*
	 *	弾生成
	 *	@param	name			弾の名前
	 *	@param	position		生成位置
	 *	@param	rotation		生成角度
	 *	@param	capsuleStart	カプセル線分の開始位置
	 *	@param	capsuleEnd		カプセル線分の終り位置
	 *  @param	capsuleRadius	カプセルの半径
	 *  @return	BulletComponentPtr
	 */
BulletComponentPtr BulletManager::GenerateBullet(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& capsuleStart,
	const Vector3& capsuleEnd,
	const float capsuleRadius) {
	// 未使用状態のオブジェクト取得
	GameObjectPtr createBullet = GameObjectManager::GetInstance().GetUnuseObject();
	if (createBullet == nullptr) return nullptr;
	// 弾生成
	BulletComponentPtr bullet = createBullet->AddComponent<BulletComponent>();
	// コライダー生成
	CapsuleColliderPtr collider = createBullet->AddComponent<CapsuleCollider>();
	Segment segment = { capsuleStart, capsuleEnd };
	collider->capsule = { segment, capsuleRadius };
	collider->isCollider = true;
	// モデルコンポーネントの追加
	auto component = createBullet->AddComponent<ModelRenderer>();
	component->SetModelHandle(bulletModel->GetHandle());
	// データのセット
	createBullet->SetObjectData(name, position, rotation);
	// シーンが持つゲームオブジェクト配列に追加
	engine->AddGameObject(createBullet);

	return bullet;
}


/*
 *	初期化
 *	@param	setEngine	エンジン
 */
void BulletManager::Initialize(Engine & setEngine) {
	engine = &setEngine;
	bulletModel = LoadManager::GetInstance().LoadResource<LoadModel>("Res/Model/Player/RIPO_BulletModel.mv1");
}

/*
 *	射撃
 *	@param	position	生成位置
 *	@param	rotation	生成角度
 *  @param	scale		サイズ
 *	@param	direction	射撃方向
 *  @param	speed		弾の速度
 *  @param	damage		弾のダメージ
 *	@param	destroyTime	弾が消える時間
 */
void BulletManager::BulletShot(
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& scale,
	const Vector3& direction,
	GameObject* shotOwner,
	const float speed,
	const float damage,
	const float destroyTime) {
	// 弾生成 
	BulletComponentPtr bullet = GenerateBullet(GameConst::_BULLET, position, rotation, BULLET_CAPSULE_START, BULLET_CAPSULE_END, BULLET_CAPSULE_RADIUS);
	// セットアップ
	bullet->Setup(direction, scale, shotOwner, damage, speed, destroyTime);
}
