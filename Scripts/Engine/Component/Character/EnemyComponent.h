/*
 *	@file	EnemyComponent.h
 *  @author kuu
 */
#ifndef _ENEMYCOMPONENT_H_
#define _ENEMYCOMPONENT_H_

#include "CharacterBase.h"
#include "../../Engine.h"

class EnemyComponent : public CharacterBase {
private:
	GameObject* enemy;
	// 移動スピード
	float moveSpeed;
	// ウェイポイント
	Vector3 wayPoint;
	// 目先のウェイポイント予定地
	Vector3 nextWayPoint;
	// ウェイポイント間の距離
	float wayPointDistance;
	// ウェイポイント追尾切り替えフラグ
	bool chaseTargetChangeFrag;
	// 方向転換時のディレイ
	float turnDelay;
	// 反転Y軸rotation
	float reverseRotationY;

	const float TOP_VALUE;

public:
	/*
	 *	コンストラクタ
	 */
	EnemyComponent();
	virtual ~EnemyComponent() = default;

public:
	/*
	 *	更新処理の前に呼び出す処理
	 */
	virtual void Start() override;

	/*
	 *	更新処理
	 *  param[in]	float	deltaTime
	 */
	virtual void Update(float deltaTime) override;

private:
	/*
	 *	移動処理
	 *  param[in]	float			deltaTime
	 */
	void EnemyMove(float deltaTime);

	/*
	 *	目標に向かって進む処理
	 *  param[in]	Vector3		wayPoint		目標の座標
	 *  param[in]	bool		targetChange	chaseTargetChangeFragの切り替え
	 *  param[in]	float		deltaTime
	 */
	void ChaseWayPoint(Vector3 wayPoint, bool targetChange, float deltaTime);

public:
	/*
	 *	ウェイポイントの位置変更
	 *  param[in]	Vector3&	setValue	敵の座標
	 */
	inline void SetWayPoint(Vector3& setValue) {
		wayPoint = Vector3(setValue.x, setValue.y, setValue.z + wayPointDistance);
		nextWayPoint = Vector3(setValue.x, setValue.y, setValue.z - wayPointDistance);
	}
};

#endif // !_ENEMYCOMPONENT_H_
