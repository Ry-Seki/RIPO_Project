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
	void WeaponInitialize() override;
	/*
	 *	更新処理
	 */
	void WeaponUpdate(float deltaTime, ActionMapBase::ActionState action, Engine* engine) override;
};

#endif // !_REVOLVERARM_H_
