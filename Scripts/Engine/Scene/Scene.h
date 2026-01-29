/*
 *  @file   Scene.h
 *  @author Seki
 */

#ifndef _SCENE_H_
#define _SCENE_H_

#include "../GameObject.h"
#include <vector>
#include <memory>
#include <variant>
#include <functional>

// 前方宣言
class Engine;

/*
 *  シーンの基底クラス
 */
class Scene {
public:
    // ワールド座標系のコライダーを表す構造体
    struct WorldCollider {
        ColliderBasePtr origin;
        std::variant<AABB, Capsule> world;
    };
    using WorldColliderPtr = std::shared_ptr<WorldCollider>;
    using WorldColliderList = std::vector<WorldColliderPtr>;

    // レイキャストの判定結果
    struct RayCastHit {
        ColliderBasePtr collider;   // 最初にレイと交差したコライダー
        Vector3 point;              // 最初のレイの座標
        float distance;             // 最初の交点までの距離
    };

    /*
     *  レイキャストの交差判定の対象になるかどうか調べる
     *  @param  collider
     *  @param  distance
     *  @return bool
     */
    using RayCastPredicate = std::function<bool(const ColliderBasePtr& collider, float distance)>;

protected:
    std::vector<GameObjectPtr> gameObjects;     // ゲームオブジェクト配列

public:
    /*
     *  コンストラクタ
     */
    Scene() = default;
    /*
     *  デストラクタ
     */
    virtual ~Scene() = default;

public:
    /*
     *  初期化処理
     */
    virtual void Initialize(Engine& engine);
    /*
     *  @brief  準備前処理
     */
    virtual void Setup(Engine& engine) {}
    /*
     *  更新処理
     */
    virtual void Update(Engine& engine, float deltaTime);
    /*
     *  描画処理
     */
    virtual void Render();
    /*
     *  破棄処理
     */
    virtual void Finalize(Engine& engine);

private:
    /*
     *  ゲームオブジェクトの衝突を処理する
     *  @author Riku
     */
    void HandleGameObjectCollision();
    /*
     *  コライダー単位の当たり判定
     *  @param  colliderA   判定対象のコライダー配列1
     *  @param  colliderB   判定対象のコライダー配列2
     *  @autohr Riku
     */
    void HandleWorldColliderCollision(
        WorldColliderList* colliderA, 
        WorldColliderList* colliderB);

public:
    /*
     *  ゲームオブジェクトの追加
     *  @param[in]  const GameObjectPtr& gameObject    ゲームオブジェクトのポインタ
     */
    void AddGameObject(const GameObjectPtr& gameObject);
    /*
     *  ゲームオブジェクトの破棄
     */
    void RemoveDestroyedObjects();
    /*
     *  全てのゲームオブジェクトの全てのコライダーをワールド座標に変換
     *  @return 変換された全オブジェクトの全コライダーのリスト
     *  @author Riku
     */
    std::vector<WorldColliderList> ChangeGameObjectWorldColliders();
    /*
     *  レイキャスト
     *  @param  ray     判定を取るレイ
     *  @param  hitInfo レイが最初にヒットしたコライダーの情報
     *  @param  pred    交差判定の条件(述語)
     *  @return bool    ヒットしたかどうか
     *  @author Riku
     */
    bool RayCast(const Ray& ray, RayCastHit& hitInfo, const RayCastPredicate& pred);

};
// 別名定義
using ScenePtr = std::shared_ptr<Scene>;

#endif // !_SCENE_H_