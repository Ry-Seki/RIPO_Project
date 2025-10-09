#include "Scene.h"
#include "../Engine.h"
#include "../ModelRenderer.h"
#include "../SpriteRenderer.h"
#include <algorithm>

void Scene::Initialize(Engine& engine) {

}

void Scene::Update(Engine& engine, float deltaTime) {
    for (auto& obj : gameObjects) {
        if (!obj->IsDestroyed()) {
            obj->Update(deltaTime);
        }
    }
    RemoveDestroyedObjects();
}

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

void Scene::Finalize(Engine& engine) {
    for (auto& obj : gameObjects) {
        if (obj) obj->OnDestroy();
    }
    gameObjects.clear();
}

void Scene::AddGameObject(const GameObjectPtr& obj) {
    if (!obj) return;
    gameObjects.push_back(obj);
    obj->Start();
}

void Scene::RemoveDestroyedObjects() {
    auto it = std::stable_partition(gameObjects.begin(), gameObjects.end(),
                                    [](const auto& obj) { return !obj->IsDestroyed(); });
    gameObjects.erase(it, gameObjects.end());
}