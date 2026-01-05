/*
 *  @file   Scene.cpp
 *  @author Seki
 */

#include "Scene.h"
#include "../Engine.h"
#include "../Component/ModelRenderer.h"
#include "../Component/SpriteRenderer.h"
#include "../Component/AABBCollider.h"
#include "../Component/CapsuleCollider.h"
#include <algorithm>

 /*
  *  初期化処理
  */
void Scene::Initialize(Engine& engine) {

}
/*
 *  更新処理
 */
void Scene::Update(Engine& engine, float deltaTime) {
	int objectsSize = gameObjects.size();
	for (int i = 0; i < objectsSize; i++) {
		auto obj = gameObjects[i];
		if (!obj->IsDestroyed()) {
			obj->Update(deltaTime);
		}
	}
	HandleGameObjectCollision();
	RemoveDestroyedObjects();
}
/*
 *  描画処理
 */
void Scene::Render() {
	for (auto& obj : gameObjects) {
		if (!obj || obj->IsDestroyed()) continue;

		for (auto& model : obj->GetComponents<ModelRenderer>()) {
			if (model) model->Render3D();
		}

		for (auto& sprite : obj->GetComponents<SpriteRenderer>()) {
			if (sprite) sprite->Render2D();
		}
	}
}
/*
 *  破棄処理
 */
void Scene::Finalize(Engine& engine) {
	for (auto& obj : gameObjects) {
		if (obj) obj->OnDestroy();
	}
	gameObjects.clear();
}
/*
 *  ゲームオブジェクトの衝突を処理する
 *  @author Riku
 */
void Scene::HandleGameObjectCollision() {
	// 全てのオブジェクトでコライダーの座標を計算
	std::vector<WorldColliderList> colliders = ChangeGameObjectWorldColliders();
	
	if (colliders.size() > 1) {
		// ゲームオブジェクト毎の衝突判定
		for (auto A = colliders.begin(); A != colliders.end() - 1; A++) {
			const GameObject* objA = A->at(0)->origin->GetOwner();
			// 削除済みは処理しない
			if (objA->IsDestroyed())
				continue;

			for (auto B = A + 1; B != colliders.end(); B++) {
				GameObject* objB = B->at(0)->origin->GetOwner();
				// 削除済みは処理しない
				if (objB->IsDestroyed())
					continue;

				// コライダー単位の衝突判定
				HandleWorldColliderCollision(&*A, &*B);
			}
		}
	}
}
/*
 *  コライダー単位の当たり判定
 *  @param  colliderA   判定対象のコライダー配列1
 *  @param  colliderB   判定対象のコライダー配列2
 *  @author Riku
 */
void Scene::HandleWorldColliderCollision(
	WorldColliderList* colliderA,
	WorldColliderList* colliderB) {
	// コライダー毎の衝突判定
	for (const auto& colA : *colliderA) {
		for (const auto& colB : *colliderB) {
			// スタティックなコライダー同士では処理しない
			if (colA->origin->isStatic && colB->origin->isStatic)
				continue;
			// どちらかが配置完了していなければ判定しない
			if (!colA->origin->isCollider || !colB->origin->isCollider)
				continue;

			// 衝突判定
			Vector3 penetration;
			if (Intersect(colA->world, colB->world, penetration)) {
				GameObject* objA = colA->origin->GetOwner();
				GameObject* objB = colB->origin->GetOwner();

				// イベントの発火
				objA->OnCollision(colA->origin, colB->origin);
				objB->OnCollision(colB->origin, colA->origin);

				// イベントの結果、どちらかが削除予定が入ったら処理を抜ける
				if (objA->IsDestroyed() || objB->IsDestroyed())
					return;
			}
		}
	}
}
/*
 *  ゲームオブジェクトの追加
 *  @param[in]  const GameObjectPtr& gameObject    ゲームオブジェクトのポインタ
 */
void Scene::AddGameObject(const GameObjectPtr& obj) {
	if (!obj) return;
	gameObjects.push_back(obj);
	obj->Start();
}
/*
 *  ゲームオブジェクトの破棄
 */
void Scene::RemoveDestroyedObjects() {
	auto it = std::stable_partition(gameObjects.begin(), gameObjects.end(),
		[](const auto& obj) { return !obj->IsDestroyed(); });
	gameObjects.erase(it, gameObjects.end());
}
/*
 *  全てのゲームオブジェクトの全てのコライダーをワールド座標に変換
 *  @return 変換された全オブジェクトの全コライダーのリスト
 *  @author Riku
 */
std::vector<Scene::WorldColliderList> Scene::ChangeGameObjectWorldColliders() {
	// ワールド座標系の衝突判定を作成
	std::vector<WorldColliderList> colliders;
	colliders.reserve(gameObjects.size());

	// 全てのゲームオブジェクトで計算
	for (const auto obj : gameObjects) {
		// コライダーを持っていないオブジェクトは処理しない
		if (obj->colliders.empty())
			continue;

		// 各コライダーのワールド座標を出す
		WorldColliderList worldList(obj->colliders.size());
		for (int i = 0; i < obj->colliders.size(); i++) {
			worldList[i] = std::make_shared<WorldCollider>();
			// AABBColliderのワールド座標変換
			if (auto aabb = std::dynamic_pointer_cast<AABBCollider>(obj->colliders[i])) {
				// オリジナルのコライダーをコピー
				worldList[i]->origin = aabb;
				// ワールド座標に変換
				AABB originAABB = aabb->aabb;
				// スケール適応
				originAABB.min = Vector3::Scale(originAABB.min, obj->scale);
				originAABB.max = Vector3::Scale(originAABB.max, obj->scale);
				// ポジション適応
				originAABB.min += obj->position;
				originAABB.max += obj->position;
				// ワールド座標保存
				worldList[i]->world = originAABB;
			}
			// CapsuleColliderのワールド座標変換
			else if (auto capsule = std::dynamic_pointer_cast<CapsuleCollider>(obj->colliders[i])) {
				// オリジナルのコライダーをコピー
				worldList[i]->origin = capsule;
				// ワールド座標に変換
				Capsule originCapsule = capsule->capsule;
				// スケール適応
				originCapsule.segment.startPoint = Vector3::Scale(originCapsule.segment.startPoint, obj->scale);
				originCapsule.segment.endPoint = Vector3::Scale(originCapsule.segment.endPoint, obj->scale);
				// カプセルの半径はy軸を参照せず、xz軸の平均を参照する
				originCapsule.radius *= (obj->scale.x + obj->scale.z) * 0.5f;
				// ポジション適応
				originCapsule.segment.startPoint += obj->position;
				originCapsule.segment.endPoint += obj->position;
				// ワールド座標保存
				worldList[i]->world = originCapsule;
			}
		}
		// 1オブジェクト内の全てのコライダーのワールド座標をリストに保存
		colliders.push_back(worldList);
	}
	return colliders;
}