/*
 *	@file	SubmachineGun.h
 *	@author Riku
 */

#ifndef _SUBMACHINEGUN_H_
#define _SUBMACHINEGUN_H_

#include "WeaponBase.h"

/*
 *	サブマシンガンのウデクラス
 */
class SubmachineGun : public WeaponBase {
public:
	SubmachineGun() = default;
	~SubmachineGun() = default;

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

#endif // !_SUBMACHINEGUN_H_
