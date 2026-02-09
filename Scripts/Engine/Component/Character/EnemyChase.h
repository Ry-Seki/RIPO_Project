/*
 *	@file	EnemyChase.h
 *  @author kuu
 */
#ifndef _ENEMYCHASE_H_
#define _ENEMYCHASE_H_

#include "EnemyComponent.h"
#include "EnemyState.h"
#include "../AnimatorComponent.h"

/*
 *	敵の追従処理クラス
 */
class EnemyChase : public EnemyState {
private:
	GameObjectPtr player;
	std::shared_ptr<AnimatorComponent> animator;
	std::shared_ptr<EnemyComponent> enemyComponent;
	// ウェイポイント
	Vector3 wayPoint;
	// 目先のウェイポイント予定地
	Vector3 nextWayPoint;
	// ウェイポイント間の距離
	float wayPointDistance;
	// 移動スピード
	float moveSpeed;
	// 視界判定の視野角
	float viewAngle;
	// 視界判定の距離
	float viewDirection;
	// プレイヤーの距離判定フラグ
	bool closePlayer;
	// プレイヤー追跡判定
	bool chasePlayer;
	// 移動量
	Vector3 moveVec;

	// 回転スピード
	const float ROTATE_SPEED;
	// プレイヤーの距離
	const float DIFFERENCE_PLAYER;

public:
	/*
	 *	コンストラクタ
	 */
	EnemyChase();
	~EnemyChase() = default;

public:
	/*
	 *	更新処理の前に呼び出す処理
	 *  param[in]	EnemyComponent&	enemy
	 */
	virtual void Start(GameObject* enemy) override;

	/*
	 *	更新処理
	 *  param[in]	GameObject*	enemy
	 *  param[in]	float		deltaTime
	 */
	virtual void Update(GameObject* enemy, float deltaTime) override;

private:
	/*
	 *	目標に向かって進む処理
	 *  param[in]	GameObject*	enemy
	 *  param[in]	Vector3		wayPoint		目標の座標
	 *  param[in]	bool		targetChange	chaseTargetChangeFragの切り替え
	 *  param[in]	float		deltaTime
	 */
	void ChaseWayPoint(GameObject* enemy, Vector3 wayPoint, bool targetChange, float deltaTime);

	/*
	 *	プレイヤーを目標に設定する
	 *  param[in]	GameObject*	enemy
	 */
	void SetTargetPlayer(GameObject* enemy, float deltaTime);

	/*
	 *	角度差を正規化
	 */
	float NormalizeAngle(float angle) {
		while (angle > Pi) angle -= 2.0f * Pi;
		while (angle < -Pi) angle += 2.0f * Pi;
		return angle;
	}
public:

};

#endif // !_ENEMYCHASE_H_
