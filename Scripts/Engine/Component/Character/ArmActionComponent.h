/*
 *	@file	ArmActionBase.h
 *	@author Riku
 */

#ifndef _ARMACTIONCOMPONENT_H_
#define _ARMACTIONCOMPONENT_H_

#include "../../Component/Component.h"
#include "../../Input/ActionMapBase.h"
#include "../../GameEnum.h"
#include "../../Input/InputUtility.h"
#include "../../Engine.h"
#include <vector>

/*
 *	ウデアクションクラス
 */
class ArmActionComponent : public Component {
private:
	std::shared_ptr<ArmActionComponent> currentArm;

protected:
	GameObject* player;
	Engine* engine;
	inline static ActionMapBase::ActionState action;

public:
	ArmActionComponent();
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

	GameObject* GetLiftObject();
};
// 別名定義
using ArmActionComponentPtr = std::shared_ptr<ArmActionComponent>;
using ArmActionComponentList = std::vector<ArmActionComponentPtr>;

#endif // !_ARMACTIONBASE_H_
