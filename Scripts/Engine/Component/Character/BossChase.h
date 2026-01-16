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
	int modelHandle;

	// 攻撃用プレイヤー検知距離
	const float PLAYER_DISTANCE;

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

};

#endif // !_BOSSCHASE_H_
