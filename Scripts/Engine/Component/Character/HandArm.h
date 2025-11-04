/*
 *	@file	HandArm.h
 *	@author Riku
 */

#ifndef _HANDARM_H_
#define _HANDARM_H_

#include "ArmActionComponent.h"
#include "../../Engine.h"

/*
 *	持ち運びができるウデクラス
 */
class HandArm : public ArmActionComponent {
private:
	GameObject* liftObject;

	const float LEFTABLE_DISTANCE;

public:
	HandArm();
	~HandArm() = default;

public:
	void Update(float deltaTime) override;

private:
	/*
	 *	お宝持ち上げ処理
	 */
	void LiftTreasure();

	/*
	 *	お宝運び処理
	 */
	void CarryTreasur();
};

#endif // !_HANDARM_H_

