/*
 *	@file	BossState.h
 *	@author kuu
 */
#ifndef _BOSSSTATE_H_
#define _BOSSSTATE_H_

#include "../../GameObject.h"

 // 前方宣言
class BossComponent;

/*
 *	敵行動パターンクラスの基底クラス
 */
class BossState {
public:
	BossState() = default;
	virtual ~BossState() = default;

	virtual void Start(GameObject* boss) {};
	virtual void Update(GameObject* boss, float deltaTime) = 0;
	virtual void Execute(BossComponent& boss) {};
};
#endif // !_BOSSSTATE_H_
