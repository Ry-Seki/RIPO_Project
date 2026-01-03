/*
 *	@file	GameObject.h
 *  @author Seki
 */

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Component/Component.h"
#include "Component/AABBCollider.h"
#include "VecMath.h"

#include <memory>
#include <vector>
#include <string>
#include <algorithm>

// 前方宣言
class Engine;

/*
 *  ゲームオブジェクトクラス
 */
class GameObject {
	friend class Engine;	// フレンドクラス
    friend class Scene;

private:
    Engine* engine = nullptr;                   // エンジン
    bool isDestroyed = false;                   // 破棄フラグ
    bool isStarted = false;                     // 開始フラグ


    std::vector<ColliderBasePtr> colliders;     // コライダーのリスト
    std::vector<ComponentPtr> components;       // コンポーネントのリスト
    std::vector<ComponentPtr> addComponents;    // 追加予定のコンポーネント

public:
    int ID;                                         // 識別ID
    std::string name;                               // オブジェクト名
    Vector3 position = { 0.0f, 0.0f, 0.0f };		// 平行移動
    Vector3 rotation = { 0.0f, 0.0f, 0.0f };		// 回転角度
    Vector3 scale = { 1.0f, 1.0f, 1.0f };			// 拡大率

public:
    GameObject() = default;
    virtual ~GameObject() = default;

public:
    // 初期化イベント
    virtual void Start();

    // 毎フレーム更新
    virtual void Update(float deltaTime);

    // 衝突イベント
    virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

    // 削除イベント
    virtual void OnDestroy();

private:
    // addComponentsにあるコンポーネントを正式に追加する
    void PushAddComponents();

public:
    // オブジェクトの情報の変更
    void SetObjectData(const std::string& setName, const Vector3& setPosition, const Vector3& setRotation);

    // コンポーネントの取得
    template <class T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
    std::shared_ptr<T> GetComponent() const {
        for (auto& component : components) {
            auto cast = std::dynamic_pointer_cast<T>(component);
            if (!cast) continue;

            return cast;
        }
        return nullptr;
    }
    // 指定した型のコンポーネント全てを取得
    template <class T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
    std::vector<std::shared_ptr<T>> GetComponents() const {
        std::vector<std::shared_ptr<T>> results;
        for (auto& component : components) {
            auto cast = std::dynamic_pointer_cast<T>(component);
            if (!cast)  continue;

            results.push_back(cast);
        }
        return results;
    }
    // コンポーネントの追加
    template <class T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
    std::shared_ptr<T> AddComponent() {
        auto component = std::make_shared<T>();
        // 所有者の設定
        component->owner = this;
        // コライダーならコライダーリストにも追加
        if constexpr (std::is_base_of_v<ColliderBase, T>)
            colliders.push_back(component);

        component->Awake();
        // コンポーネントリストに追加
        components.push_back(component);
        return component;
    }
    // コンポーネントの追加(Update用)
    template <class T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
    std::shared_ptr<T> AddCompoentUpdate() {
        auto component = std::make_shared<T>();
        // 所有者の設定
        component->owner = this;
        // コライダーならコライダーリストにも追加
        if constexpr (std::is_base_of_v<ColliderBase, T>)
            colliders.push_back(component);

        component->Awake();
        // 追加予定のコンポーネントリストに追加
        addComponents.push_back(component);
        return component;
    }
    // コンポーネント削除
    void RemoveComponent();

    // ゲームオブジェクトのステータスのリセット
    void ResetGameObject();

public:
    /*
     *  エンジンの取得
     *  @return Engine*
     */
    inline Engine* GetEngine() const { return engine; }
    /*
     *  破棄フラグの取得
     *  @return bool
     */
    inline bool IsDestroyed() const { return isDestroyed; }
    /*
     *  破棄フラグをtrueにする
     */
    inline void Destroy() { isDestroyed = true; }
    /*
     *  破棄フラグの初期化
     */
    inline void ResetDestroy() { isDestroyed = false; }
};
//	別名定義
using GameObjectPtr = std::shared_ptr<GameObject>;
using GameObjectList = std::vector<GameObjectPtr>;

#endif // !_GAMEOBJECT_H_