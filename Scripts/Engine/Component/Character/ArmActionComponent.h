/*
 *	@file	ArmActionBase.h
 *	@author Riku
 */

#ifndef _ARMACTIONCOMPONENT_H_
#define _ARMACTIONCOMPONENT_H_

#include "../../Component/Component.h"
#include "../../Engine.h"
#include "HandArm.h"
#include <vector>

/*
 *	ウデアクションクラス
 */
class ArmActionComponent : public Component {
private:
	std::shared_ptr<ArmActionComponent> currentArm = nullptr;

protected:
	GameObject* player;
	Engine* engine;

public:
	ArmActionComponent() = default;
	virtual ~ArmActionComponent() = default;

public:
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;

private:
	virtual void ArmUpdate(float deltaTime, GameObject* player, Engine* engine){}

public:
	/*
	 *	使用するウデをセット
	 */
	template <class T>
	void SetCurrentArm() {
		currentArm = std::make_shared<T>();
	}

	/*
	 *	運んでいるオブジェクトを取得
	 */
	GameObject* GetLiftObject() { 
		// currentArmがHandArm型でないならnullptrが代入される(dynamic_pointer_castの仕様)
		if (auto arm = std::dynamic_pointer_cast<HandArm>(currentArm))
			return arm->GetLiftObject();
		return nullptr;
	}
};
// 別名定義
using ArmActionComponentPtr = std::shared_ptr<ArmActionComponent>;
using ArmActionComponentList = std::vector<ArmActionComponentPtr>;

#endif // !_ARMACTIONBASE_H_
