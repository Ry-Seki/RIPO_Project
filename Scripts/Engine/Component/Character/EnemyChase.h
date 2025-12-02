/*
 *	@file	EnemyChase.h
 *  @author kuu
 */
#ifndef _ENEMYCHASE_H_
#define _ENEMYCHASE_H_

#include "EnemyComponent.h"

/*
 *	敵の追従処理クラス
 */
class EnemyChase : public EnemyComponent {
private:
	GameObject* enemy;
	GameObjectPtr player;
	// ウェイポイント
	Vector3 wayPoint;
	// 目先のウェイポイント予定地
	Vector3 nextWayPoint;
	// ウェイポイント間の距離
	float wayPointDistance;
	// プレイヤーの距離判定フラグ
	bool closePlayer;
	// 移動量
	Vector3 moveVec;

	// 移動スピード
	const float MOVE_SPEED;
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
	 */
	virtual void Start() override;

	/*
	 *	更新処理
	 *  param[in]	float	deltaTime
	 */
	virtual void Update(float deltaTime) override;

private:
	/*
	 *	目標に向かって進む処理
	 *  param[in]	Vector3		wayPoint		目標の座標
	 *  param[in]	bool		targetChange	chaseTargetChangeFragの切り替え
	 *  param[in]	float		deltaTime
	 */
	void ChaseWayPoint(Vector3 wayPoint, bool targetChange, float deltaTime);

public:

};

#endif // !_ENEMYCHASE_H_
