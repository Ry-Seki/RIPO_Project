/*
 *	@file	HandArm.h
 *	@author Riku
 */

#ifndef _HANDARM_H_
#define _HANDARM_H_

#include "ArmActionComponent.h"

/*
 *	持ち運びができるウデクラス
 */
class HandArm : public ArmActionComponent {
private:

public:
	HandArm() = default;
	~HandArm() = default;

public:
	void Update(float deltaTime);
};

#endif // !_HANDARM_H_

