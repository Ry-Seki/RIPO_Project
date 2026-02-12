/*
 *	@file	ArmBase.h
 *  @author Riku
 */
#ifndef _ARM_BASE_H_
#define _ARM_BASE_H_

#include "../../Input/ActionMapBase.h"
#include "../../Engine.h"
#include <memory>

/*
 * ウデの基底クラス
 */
class ArmBase {

public:
	ArmBase() = default;
	~ArmBase() = default;

public:
	/*
	 *	最初のUpdateの直前に呼び出される処理
	 */
	virtual void Start() {}
	/*
	 *	初期化処理
	 */
	virtual void Initialize() {}
	/*
	 *	更新処理
	 */
	virtual void ArmUpdate(float deltaTime, ActionMapBase::ActionState action, Engine* engine) {}
};
// 別名定義
using ArmBasePtr = std::shared_ptr<ArmBase>;

#endif // !_ARM_BASE_H_
