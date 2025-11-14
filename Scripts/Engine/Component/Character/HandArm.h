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
	GameObject* liftObject;			// 運ぶオブジェクト

	const float LEFTABLE_DISTANCE;

public:
	HandArm();
	~HandArm() = default;

public:
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other);
	void ArmUpdate(float deltaTime, GameObject* player, Engine* engine) override;

private:
	/*
	 *	お宝持ち上げ処理
	 */
	void LiftTreasure(GameObject* player, Engine* engine);

	/*
	 *	お宝運び処理
	 */
	void CarryTreasur(GameObject* player);

public:
	inline GameObject* GetLiftObject() const { return liftObject; }
};

#endif // !_HANDARM_H_

