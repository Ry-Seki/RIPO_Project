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
	bool isShot;						// 射撃可能か否か
	float shotCoolTime;					// 射撃のクールタイム

	float const SHOT_COOL_TIME_MAX;		// 射撃のクールタイムの最大

public:
	RevolverArm();
	~RevolverArm() = default;

public:
	void ArmUpdate(float deltaTime, GameObject* player, Engine* engine) override;
};

#endif // !_REVOLVERARM_H_
