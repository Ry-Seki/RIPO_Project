/*
 *	@file	RevolverArm.h
 *  @author Riku
 */

#ifndef _REVOLVERARM_H_
#define _REVOLVERARM_H_

#include "WeaponBase.h"

/*
 *	リボルバーのウデクラス
 */
class RevolverArm : public WeaponBase {
private:
	float shotCoolTime;					// 射撃のクールタイム

	float const SHOT_COOL_TIME_MAX;		// 射撃のクールタイムの最大

public:
	RevolverArm();
	~RevolverArm() = default;

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

#endif // !_REVOLVERARM_H_
