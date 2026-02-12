/*
 *	@file	BossChase
 *	@author	kuu
 */
#ifndef _BOSSCHASE_H_
#define	_BOSSCHASE_H_

#include "BossState.h"
#include "../AnimatorComponent.h"

 /*
  *	追跡状態処理
  */
class BossChase : public BossState {
private:
	GameObjectPtr player;
	std::shared_ptr<AnimatorComponent> animator;
	// モデルハンドル
	int modelHandle;

	// 近距離攻撃用プレイヤー検知距離
	const float PLAYER_DISTANCE;
	// 遠距離攻撃用プレイヤー検知距離
	const float SHOOTING_PLAYER_DISTANCE;
	// 回転スピード
	const float ROTATE_SPEED;
	// 移動スピード
	const float MOVE_SPEED;

public:
	/*
	 *	コンストラクタ
	 */
	BossChase();
	~BossChase() = default;

public:
	/*
	 *	更新処理の前に呼び出す処理
	 *  param[in]	BossComponent&	boss
	 */
	virtual void Start(GameObject* boss) override;

	/*
	 *	更新処理
	 *  param[in]	GameObject*	boss
	 *  param[in]	float		deltaTime
	 */
	virtual void Update(GameObject* boss, float deltaTime) override;

private:
	/*
	 *	目標に向かって進む処理
	 *  param[in]	GameObject*	boss
	 *  param[in]	Vector3		wayPoint		目標の座標
	 *  param[in]	bool		targetChange	chaseTargetChangeFragの切り替え
	 *  param[in]	float		deltaTime
	 */
	void ChaseWayPoint(GameObject* boss, Vector3 wayPoint, float deltaTime);
};

#endif // !_BOSSCHASE_H_
