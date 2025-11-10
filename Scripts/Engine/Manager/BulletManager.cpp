/*
 *	@file	BulletManager.cpp
 *  @author	Riku
 */

#include "BulletManager.h"
#include "GameObjectManager.h"
#include "../Component/Character/BulletComponent.h"
#include "../Component/AABBCollider.h"
#include "../Component/ModelRenderer.h"

BulletManager::BulletManager()
	: engine(nullptr)
{}

void BulletManager::Initialize(Engine & setEngine) {
	engine = &setEngine;
}

/*
 *	弾生成
 *	@param	name		弾の名前
 *	@param	position	生成位置
 *	@param	rotation	生成角度
 *	@param	AABBMin		AABBの各軸における最小値
 *	@param	AABBMax		AABBの各軸における最大値
 */
void BulletManager::GenerateBullet(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax) {
	// 未使用状態のオブジェクト取得
	GameObjectPtr createBullet = GameObjectManager::GetInstance().GetUnuseObject();
	if (createBullet == nullptr) return;
	// 弾生成
	createBullet->AddComponent<BulletComponent>();
	// コライダー生成
	AABBColliderPtr collider = createBullet->AddComponent<AABBCollider>();
	collider->aabb = { AABBMin, AABBMax };
	// モデルコンポーネントの追加
	createBullet->AddComponent<ModelRenderer>();
	// データのセット
	createBullet->SetObjectData(name, position, rotation);
	// シーンが持つゲームオブジェクト配列に追加
	engine->AddGameObject(createBullet);
}
