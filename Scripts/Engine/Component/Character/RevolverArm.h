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
