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


public:
	// コンストラクタ
	EnemyComponent();
	virtual ~EnemyComponent() = default;

public:
	// 更新処理
	virtual void Update(float deltaTime) override;
};

#endif // !_ENEMYCOMPONENT_H_
