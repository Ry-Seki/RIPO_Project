/*
 *	@file	ArmActionBase.cpp
 *	@author Riku
 */

#include "ArmActionComponent.h"
#include "../../GameConst.h"
#include "HandArm.h"
#include "WeaponBase.h"

using namespace InputUtility;

/*
 *	最初のUpdateの直前に呼び出される処理
 */
void ArmActionComponent::Start() {
	arms[GameEnum::Arm::Hand] = std::make_shared<HandArm>();
	arms[GameEnum::Arm::Hand]->Start();
	arms[GameEnum::Arm::Weapon] = std::make_shared<WeaponBase>();
	arms[GameEnum::Arm::Weapon]->Start();
}

/*
 *	更新処理
 */
void ArmActionComponent::Update(float deltaTime) {
	action = GetInputState(GameEnum::ActionMap::PlayerAction);
	auto engine = GetOwner()->GetEngine();
	// 両ウデの更新処理
	for (auto [armNum, arm] : arms) {
		// 物を持ってるときは武器の更新はスキップ
		if (arm == arms[GameEnum::Arm::Weapon] && GetLiftObject())
			continue;
		arm->ArmUpdate(deltaTime, action, engine);
	}
}


