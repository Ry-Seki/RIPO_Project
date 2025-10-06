/*
 *	@file	GameObject.h
 */

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Component.h"
#include "VecMath.h"

#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include <algorithm>

// 前方宣言
class Engine;

class GameObject {
	friend class Engine;	// フレンドクラス

private:
    Engine* engine = nullptr;
    bool isDestroyed = false;
    bool isStarted = false;

    std::vector<ComponentPtr> components;    // コンポーネントのリスト
    std::vector<ComponentPtr> pendingAdd;    // 追加予定のコンポーネント

public:
    std::string name;
    Vector3 position = { 0.0f, 0.0f, 0.0f };		// 平行移動
    Vector3 rotation = { 0.0f, 0.0f, 0.0f };		// 回転角度
    Vector3 scale = { 1.0f, 1.0f, 1.0f };			// 拡大率

public:
    GameObject() = default;
    virtual ~GameObject() = default;

protected:
    // 初期化イベント
    virtual void Start();

    // 毎フレーム更新
    virtual void Update(float deltaTime);

    // 描画処理
    virtual void Render();

    // 衝突イベント
    virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

    // 削除イベント
    virtual void OnDestroy();

public:
    // コンポーネント追加
    template <class T>
    std::shared_ptr<T> AddComponent() {
        auto component = std::make_shared<T>();
        // 所有者をセット
        component->owner = this;

        // Awakeの呼び出し
        component->Awake();

        // 現在のフレームではまだ追加しない(安全性のため)
        pendingAdd.push_back(component);
        return component;
    }
    // コンポーネント削除
    void RemoveComponent();

private:
    // pendingAddにあるコンポーネントを正式に追加する
    void FlushPendingComponents();

public:
    inline Engine* GetEngine() const { return engine; }
    inline bool IsDestroyed() const { return isDestroyed; }
    inline void Destroy() { isDestroyed = true; }

};
//	別名定義
using GameObjectPtr = std::shared_ptr<GameObject>;
using GameObjectList = std::vector<GameObjectPtr>;

#endif // !_GAMEOBJECT_H_