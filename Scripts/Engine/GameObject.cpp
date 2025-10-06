#include "GameObject.h"

/*
 *  初期化処理
 */
void GameObject::Start() {
    // Start関数は一度だけ呼ばれるようにする
    if (isStarted) return;
    isStarted = true;
    for (auto& component : components) {
        if (!component->isStarted) {
            component->Start();
            component->isStarted = true;
        }
    }
}
/*
 *  更新処理
 */
void GameObject::Update(float deltaTime) {
    // 既存コンポーネントの更新
    for (auto& component : components) if (!component->IsDestroyed()) component->Update(deltaTime);
    // 追加待機中のコンポーネントを反映
    FlushPendingComponents();
    // 削除予定コンポーネントを整理
    RemoveComponent();
}
/*
 *  衝突判定
 */
void GameObject::OnCollision(const ComponentPtr& self, const ComponentPtr& other) {
    for (auto& component : components) component->OnCollision(self, other);
}
/*
 *  破棄処理
 */
void GameObject::OnDestroy() {
    for (auto& component : components) component->OnDestroy();
}
/*
 *  コンポーネント破棄処理
 */
void GameObject::RemoveComponent() {
    if (components.empty()) return;

    const auto itr = std::stable_partition(
        components.begin(), components.end(),
        [](const auto& p) { return !p->IsDestroyed(); });

    std::vector<ComponentPtr> destroyList(
        std::move_iterator(itr),
        std::move_iterator(components.end()));
    components.erase(itr, components.end());

    for (auto& c : destroyList) c->OnDestroy();
}
/*
 *  Update中に追加されたコンポーネントの反映
 */
void GameObject::FlushPendingComponents() {
    if (pendingAdd.empty()) return;

    for (auto& component : pendingAdd) {
        components.push_back(component);

        // 追加直後にStart()も呼ぶ
        if (!component->isStarted) {
            component->Start();
            component->isStarted = true;
        }
    }
    pendingAdd.clear();
}