/*
 *	@file	SubmachineGunArm.h
 *	@author Riku
 */

#ifndef _SUBMACHINEGUNARM_H_
#define _SUBMACHINEGUNARM_H_

#include "WeaponBase.h"

/*
 *	サブマシンガンのウデクラス
 */
class SubmachineGunArm : public WeaponBase {
public:
	SubmachineGunArm() = default;
	~SubmachineGunArm() = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize() override;
	/*
	 *	更新処理
	 */
	void ArmUpdate(float deltaTime, ActionMapBase::ActionState action) override;
};

#endif // !_SUBMACHINEGUNARM_H_
