/*
 *	@file	ArmActionBase.cpp
 *	@author Riku
 */

#include "ArmActionComponent.h"
#include "../../GameConst.h"

void ArmActionComponent::Start() {
	player = GetOwner();
	engine = player->GetEngine();
}

void ArmActionComponent::Update(float deltaTime) {
	if (currentArm == nullptr) return;
	currentArm->ArmUpdate(deltaTime, player, engine);
}

void ArmActionComponent::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	if (currentArm == nullptr) return;
	currentArm->OnCollision(self, other);
}
