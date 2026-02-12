/*
 *	@file	BulletManager.cpp
 *  @author	Riku
 */

#include "BulletManager.h"
#include "GameObjectManager.h"
#include "../Component/AABBCollider.h"
#include "../Component/ModelRenderer.h"
#include "../Load/LoadManager.h"

#include <DxLib.h>

BulletManager::BulletManager()
	: engine(nullptr)

	, BULLET_NAME("bullet")
	, BULLET_AABB_MIN({ -10, 0, -10 })
	, BULLET_AABB_MAX({ 10, 20, 10 })
{}

/*
 *	弾生成
 *	@param	name		弾の名前
 *	@param	position	生成位置
 *	@param	rotation	生成角度
 *	@param	AABBMin		AABBの各軸における最小値
 *	@param	AABBMax		AABBの各軸における最大値
 *  @return	BulletComponentPtr
 */
BulletComponentPtr BulletManager::GenerateBullet(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax) {
	// 未使用状態のオブジェクト取得
	GameObjectPtr createBullet = GameObjectManager::GetInstance().GetUnuseObject();
	if (createBullet == nullptr) return nullptr;
	// 弾生成
	BulletComponentPtr bullet = createBullet->AddComponent<BulletComponent>();
	// コライダー生成
	AABBColliderPtr collider = createBullet->AddComponent<AABBCollider>();
	collider->aabb = { AABBMin, AABBMax };
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
 */
void BulletManager::BulletShot(
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& scale,
	const Vector3& direction,
	const GameObjectPtr& shotOwner,
	const float speed,
	const float damage) {
	// 弾生成 
	BulletComponentPtr bullet = GenerateBullet(BULLET_NAME, position, rotation, BULLET_AABB_MIN, BULLET_AABB_MAX);
	// セットアップ
	bullet->Setup(direction, scale, shotOwner, damage, speed);
}

