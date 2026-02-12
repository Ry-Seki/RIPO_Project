/*
 *	@file	HandArm.h
 *	@author Riku
 */

#ifndef _HANDARM_H_
#define _HANDARM_H_

#include "ArmBase.h"
#include "../../Engine.h"

/*
 *	持ち運びができるウデクラス
 */
class HandArm : public ArmBase {
private:
	GameObject* liftObject;			// 運ぶオブジェクト

	const float LEFTABLE_DISTANCE;	// 運べる距離

public:
	HandArm();
	~HandArm() = default;

public:
	/*
	 *	更新処理
	 */
	void ArmUpdate(float deltaTime, ActionMapBase::ActionState action, Engine* engine) override;

private:
	/*
	 *	お宝持ち上げ処理
	 */
	void LiftTreasure(GameObjectPtr player, Engine* engine);

	/*
	 *	お宝運び処理
	 */
	void CarryTreasure(GameObjectPtr player);

public:
	inline GameObject* GetLiftObject() const { return liftObject; }
};

#endif // !_HANDARM_H_

