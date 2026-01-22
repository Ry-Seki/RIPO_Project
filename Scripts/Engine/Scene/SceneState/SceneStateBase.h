/*
 *	@file	SceneStateBase.h
 *	@author	Seki
 */

#ifndef _SCENE_STATE_BASE_H_
#define _SCENE_STATE_BASE_H_

#include "../../GameObject.h"

#include <memory>
#include <vector>
#include <variant>
#include <functional>
#include <algorithm>

// 前方宣言
class Engine;

/*
 *	@brief	シーンステートの基底クラス
 */
class SceneStateBase {
protected:
	GameObjectList gameObjects;     // ゲームオブジェクト配列

public:
    /*
     *  @brief  ワールド座標系のコライダーを表す構造体
     *  @author Riku
     */
    struct WorldCollider {
        ColliderBasePtr origin;
        std::variant<AABB, Capsule> world;
    };
public:    // 別名定義
    using WorldColliderPtr = std::shared_ptr<WorldCollider>;
    using WorldColliderList = std::vector<WorldColliderPtr>;

public:
    /*
     *  @brief  レイキャストの判定結果構造体
     *  @author Riku
     */
    struct RaycastHit {
        ColliderBasePtr collider;   // 最初にレイと交差したコライダー
        Vector3 point;              // 最初のレイの座標
        float distance;             // 最初の交点までの距離
    };

public:     // 別名定義
    /*
     *  @brief      レイキャストの交差判定の対象になるかどうか調べる
     *  @author     Riku
     *  @param[in]  const ColliderBasePtr& collider
     *  @param[in]  floatdistance
     *  @return     bool
     */
    using RaycastPredicate = std::function<bool(const ColliderBasePtr& collider, float distance)>;

public:
	/*
	 *	@brief	デストラクタ
	 */
	virtual ~SceneStateBase() {}

public:
    /*
     *  @brief  初期化処理
     */
    virtual void Initialize(Engine& engine) {}
    /*
     *  @brief  準備前処理
     */
    virtual void Setup(Engine& engine) {}
    /*
     *  @brief  更新処理
     */
    virtual void Update(Engine& engine, float deltaTime);
    /*
     *  @brief  描画処理
     */
    virtual void Render();
    /*
     *  @biref  片付け処理
     */
    virtual void Teardown(Engine& engine);

private:
    /*
     *  @brief      ゲームオブジェクトの衝突を処理する
     *  @author     Riku
     */
    void HandleGameObjectCollision();
    /*
     *  @brief      コライダー単位の当たり判定
     *  @autohr     Riku
     *  @param[in]  WorldColliderList* colliderA   判定対象のコライダー配列1
     *  @param[in]  WorldColliderList * colliderB   判定対象のコライダー配列2
     */
    void HandleWorldColliderCollision(
        WorldColliderList* colliderA,
        WorldColliderList* colliderB);

public:
    /*
     *  @brief      ゲームオブジェクトの追加
     *  @param[in]  const GameObjectPtr& gameObject    ゲームオブジェクトのポインタ
     */
    void AddGameObject(const GameObjectPtr& gameObject);
    /*
     *  @brief      ゲームオブジェクトの破棄
     */
    void RemoveDestroyedObjects();
    /*
     *  @brief  全てのゲームオブジェクトの全てのコライダーをワールド座標に変換
     *  @author Riku
     *  @return 変換された全オブジェクトの全コライダーのリスト
     */
    std::vector<WorldColliderList> ChangeGameObjectWorldColliders();
    /*
     *  @brief      レイキャスト
     *  @author     Riku
     *  @param[in]  const Ray& ray                 判定を取るレイ
     *  @param[out] RaycastHit& hitInfo            レイが最初にヒットしたコライダーの情報
     *  @param[in]  const RaycastPredicate& pred   交差判定の条件(述語)
     *  @return     bool    ヒットしたかどうか
     */
    bool Raycast(const Ray& ray, RaycastHit& hitInfo, const RaycastPredicate& pred);

};

#endif // !_SCENE_STATE_BASE_H_