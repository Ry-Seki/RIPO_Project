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
