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