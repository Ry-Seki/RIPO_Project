/*
 *	@file	ArmActionBase.h
 *	@author Riku
 */

#ifndef _ARMACTIONBASE_H_
#define _ARMACTIONBASE_H_

#include "../../Component/Component.h"
#include <vector>

/*
 *	ウデアクションの基底クラス
 */
class ArmActionBase : public Component {

};
// 別名定義
using ArmActionBasePtr = std::shared_ptr<ArmActionBase>;
using ArmActionBaseList = std::vector<ArmActionBasePtr>;

#endif // !_ARMACTIONBASE_H_
