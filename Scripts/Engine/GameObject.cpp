#include "GameObject.h"
#include <DxLib.h>
#include <algorithm>
#include <vector>
#include <memory>

/*
 *  初期化処理
 */
void GameObject::Start() {
    if (isStarted) return;
    isStarted = true;

    for (auto& component : components) {
        if (component && !component->isStarted) {
            component->Start();
            component->isStarted = true;
        }
    }
}

/*
 *  更新処理
 */
void GameObject::Update(float deltaTime) {
    // 既存コンポーネントの更新（破棄済みはスキップ）
    for (auto& component : components) {
        if (component && !component->IsDestroyed()) {
            component->Update(deltaTime);
        }
    }
    // フレーム中に追加されたコンポーネントを即反映
    PushAddComponents();
    // 破棄対象のコンポーネント整理
    RemoveComponent();
}

/*
 *  衝突判定
 */
void GameObject::OnCollision(const ComponentPtr& self, const ComponentPtr& other) {
    for (auto& component : components) {
        if (component) component->OnCollision(self, other);
    }
}

/*
 *  破棄処理
 */
void GameObject::OnDestroy() {
    for (auto& component : components) {
        if (component) component->OnDestroy();
    }
    components.clear();
    addComponents.clear();
}

/*
 *  コンポーネント破棄処理
 */
void GameObject::RemoveComponent() {
    if (components.empty()) return;

    // 安全に削除対象を収集
    std::vector<ComponentPtr> destroyList;
    destroyList.reserve(components.size());

    auto it = std::stable_partition(components.begin(), components.end(),
                                    [&destroyList](const auto& component) {
        if (!component || component->IsDestroyed()) {
            if (component) destroyList.push_back(component);

            return false; // 削除対象
        }
        return true; // 残す
    });

    // 残すコンポーネントだけに更新
    components.erase(it, components.end());

    // 破棄処理呼び出し（軽量化のため移動せず参照で呼ぶ）
    for (auto& component : destroyList) {
        component->OnDestroy();
    }
}

/*
 *  Update中に追加されたコンポーネントを即時反映
 */
void GameObject::PushAddComponents() {
    if (addComponents.empty()) return;

    for (auto& component : addComponents) {
        if (!component) continue;

        components.push_back(component);

        if (!component->isStarted) {
            component->Start();
            component->isStarted = true;
        }
    }

    addComponents.clear();
}