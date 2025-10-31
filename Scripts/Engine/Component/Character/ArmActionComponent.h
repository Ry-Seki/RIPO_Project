/*
 *	@file	ArmActionBase.h
 *	@author Riku
 */

#ifndef _ARMACTIONCOMPONENT_H_
#define _ARMACTIONCOMPONENT_H_

#include "../../Component/Component.h"
#include <vector>

/*
 *	ウデアクションクラス
 */
class ArmActionComponent : public Component {
private:
	std::shared_ptr<ArmActionComponent> currentArm;

public:
	ArmActionComponent() = default;
	virtual ~ArmActionComponent() = default;

public:
	virtual void Update(float deltaTime) override {
		currentArm->Update(deltaTime);
	}

public:
	/*
	 *	使用するウデをセット
	 */
	template <class T>
	void SetCurrentArm() {
		currentArm = std::make_shared<T>();
	}
};
// 別名定義
using ArmActionComponentPtr = std::shared_ptr<ArmActionComponent>;
using ArmActionComponentList = std::vector<ArmActionComponentPtr>;

#endif // !_ARMACTIONBASE_H_
