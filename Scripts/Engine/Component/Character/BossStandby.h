/*
 *	@file	BossStandby
 *	@author	kuu
 */
#ifndef _BOSSSTANDBY_H_
#define _BOSSSTANDBY_H_

#include "BossState.h"
#include "../AnimatorComponent.h"

/*
 *	待機状態処理
 */
class BossStandby : public BossState {
public:
	/*
	 *	コンストラクタ
	 */
	BossStandby();
	~BossStandby() = default;

private:
	GameObjectPtr player;
	std::shared_ptr<AnimatorComponent> animator;
	int modelHandle;
	// 視野角の半径
	float viewAngle;

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
#endif // !_BOSSSTANDBY_H_
