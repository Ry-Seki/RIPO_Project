/*
 *	@file	BossComponent.h
 *  @author kuu
 */
#ifndef _BOSSCOMPONENT_H_
#define _BOSSCOMPONENT_H_

#include "CharacterBase.h"

class BossComponent : public CharacterBase {
private:

public:
	/*
	 *	コンストラクタ
	 */
	BossComponent();
	~BossComponent() = default;

public:
	/*
	 *	更新処理
	 */
	virtual void Update(float deltaTime) override;

};

#endif // !_BOSSCOMPONENT_H_