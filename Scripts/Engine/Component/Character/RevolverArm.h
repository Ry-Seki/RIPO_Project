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
public:
	RevolverArm() = default;
	~RevolverArm() = default;

public:
	void ArmUpdate(float deltaTime, GameObject* player, Engine* engine) override;
};

#endif // !_REVOLVERARM_H_
