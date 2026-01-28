/*
 *	@file	BossDeath.h
 *	@author	kuu
 */
#ifndef _BOSSDEATH_H_
#define _BOSSDEATH_H_

#include "BossState.h"
#include "../AnimatorComponent.h"
#include "BossComponent.h"

class BossDeath : public BossState {
private:
	std::shared_ptr<AnimatorComponent> animator;
	// アニメーション経過時間
	float animationTime;

public:
	/*
	 *	コンストラクタ
	 */
	BossDeath();
	~BossDeath() = default;

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

#endif // !_BOSSDEATH_H_
