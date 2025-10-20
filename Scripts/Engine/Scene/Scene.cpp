/*
 *  @file   Scene.cpp
 *  @author Seki
 */

#include "Scene.h"
#include "../Engine.h"
#include "../Component/ModelRenderer.h"
#include "../Component/SpriteRenderer.h"
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
    for (auto& obj : gameObjects) {
        if (!obj->IsDestroyed()) {
            obj->Update(deltaTime);
        }
    }
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
 */
void Scene::HandleGameObjectCollision() {
    // ワールド座標系の衝突判定を作成
    std::vector<WorldColliderList> colliders;
    colliders.reserve(gameObjects.size());

    // 全てのオブジェクトでコライダーの座標を計算
    for (const auto obj : gameObjects) {
        continue;

        WorldColliderList list(obj->colliders.size());

        for (int i = 0; i < obj->colliders.size(); i++) {
            // オリジナルのコライダーをコピー
            list[i].origin = obj->colliders[i];
            list[i].world = obj->colliders[i]->aabb;

            list[i].world.min = Vector3::Scale(list[i].world.min, obj->scale);
            list[i].world.max = Vector3::Scale(list[i].world.max, obj->scale);
            list[i].world.min += obj->position;
            list[i].world.max += obj->position;
        }
        colliders.push_back(list);
    }
    if (colliders.size() >= 2) {
        // ゲームオブジェクト毎の衝突判定
        for (auto A = colliders.begin(); A != colliders.end() - 1; A++) {
            const GameObject* objA = A->at(0).origin->GetOwner();
            // 削除済みは処理しない
            if (objA->IsDestroyed())
                continue;

            for (auto B = A + 1; B != colliders.end(); B++) {
                const GameObject* objB = B->at(0).origin->GetOwner();
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
 */
void Scene::HandleWorldColliderCollision(
    WorldColliderList* colliderA,
    WorldColliderList* colliderB) {
    // コライダー毎の衝突判定
    for (const auto& colA : *colliderA) {
        for (const auto& colB : *colliderB) {
            // スタティックなコライダー同士では処理しない
            if (colA.origin->isStatic && colB.origin->isStatic)
                continue;

            // 衝突判定
            Vector3 penetration;
            if (Intersect(colA.world, colB.world, penetration)) {
                GameObject* objA = colA.origin->GetOwner();
                GameObject* objB = colB.origin->GetOwner();

                // イベントの発火
                objA->OnCollision(colA.origin, colB.origin);
                objB->OnCollision(colB.origin, colA.origin);

                // イベントの結果、どちらかが削除予定が入ったら処理を抜ける
                if (objA->IsDestroyed() || objB->IsDestroyed())
                    return;
            }
        }
    }
}
/*
 *  ゲームオブジェクトの追加
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