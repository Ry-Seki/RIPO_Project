/*
 *	@file	BossComponent.h
 *  @author kuu
 */
#ifndef _BOSSCOMPONENT_H_
#define _BOSSCOMPONENT_H_

#include "CharacterBase.h"
#include "BossState.h"

class BossComponent : public CharacterBase {
private:
	GameObject* boss;
	BossState* state;
	int animationHandle;
	int modelHandle;

public:
	/*
	 *	コンストラクタ
	 */
	BossComponent();
	BossComponent(BossState* initState);
	~BossComponent();

public:
	virtual void Start() override;
	/*
	 *	更新処理
	 */
	virtual void Update(float deltaTime) override;

};

#endif // !_BOSSCOMPONENT_H_