/*
 *	MoveComponent.cpp
 *	@author Riku
 */

#include "MoveComponent.h"
#include "../GameObject.h"

MoveComponent::MoveComponent()
	: velocity(V_ZERO)
{}

/*
 *	更新処理
 */
void MoveComponent::Update(float deltaTime) {
	// オーナを動かす
	auto owner = GetOwner();
	owner->position = velocity * deltaTime;

	velocity *= 0.9f;
}

/*
 *	速度ベクトルをセット
 */
void MoveComponent::SetVelocity(Vector3 direction, float speed) {
	velocity = direction * speed;
}
