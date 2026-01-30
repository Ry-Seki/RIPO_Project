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
#include "ArmBase.h"
#include "HandArm.h"
#include <vector>
#include <unordered_map>

/*
 *	ウデアクションクラス
 */
class ArmActionComponent : public Component {
private:
	ArmBasePtr currentArm;								// 現在使っているウデ
	std::unordered_map<GameEnum::Arm, ArmBasePtr> arms;	// 使用可能なウデ達
	ActionMapBase::ActionState action;					// 入力状態

public:
	ArmActionComponent();
	virtual ~ArmActionComponent() = default;

public:
	/*
	 *	最初のUpdateの直前に呼び出される処理
	 */
	void Start() override;
	/*
	 *	更新処理
	 */
	void Update(float deltaTime) override;

public:
	/*
	 *	使用するウデをセット
	 */
	inline void SetCurrentArm(GameEnum::Arm setArm) {
		if (currentArm == arms[setArm])
			return;
		currentArm = arms[setArm];
		currentArm->Initialize();
	}

	/*
	 *	運んでいるオブジェクトを取得
	 */
	inline GameObject* GetLiftObject() {
		// currentArmがHandArm型でないならnullptrが代入される(dynamic_pointer_castの仕様)
		if (auto arm = std::dynamic_pointer_cast<HandArm>(currentArm))
			return arm->GetLiftObject();
		return nullptr;
	}
};
// 別名定義
using ArmActionComponentPtr = std::shared_ptr<ArmActionComponent>;

#endif // !_ARMACTIONBASE_H_
