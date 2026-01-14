/*
 *	@file	ArmActionBase.cpp
 *	@author Riku
 */

#include "ArmActionComponent.h"
#include "../../GameConst.h"
#include "HandArm.h"

void ArmActionComponent::Start() {
	player = GetOwner();
	engine = player->GetEngine();
}

void ArmActionComponent::Update(float deltaTime) {
	if (currentArm == nullptr)
		return;
	currentArm->ArmUpdate(deltaTime, player, engine);
}

void ArmActionComponent::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	if (currentArm == nullptr) 
		return;
	currentArm->OnCollision(self, other);
}

/*
 *	運んでいるオブジェクトを取得
 */
GameObject* ArmActionComponent::GetLiftObject() {
	// currentArmがHandArm型でないならnullptrが代入される(dynamic_pointer_castの仕様)
	if (auto arm = std::dynamic_pointer_cast<HandArm>(currentArm))
		return arm->GetLiftObject();
	return nullptr;
}
