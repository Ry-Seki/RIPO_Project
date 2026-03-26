/*
 *	MoveComponent.cpp
 *	@author Riku
 */

#include "MoveComponent.h"
#include "../GameObject.h"

MoveComponent::MoveComponent()
	: velocity(V_ZERO)
	, moveVec(V_ZERO)
	, isMove(false)

	, INERTIA_RATE(0.75f)
{}

/*
 *	更新処理
 */
void MoveComponent::Update(float deltaTime) {
	// オーナを動かす
	auto owner = GetOwner();
	moveVec = velocity * deltaTime;
	owner->position += moveVec;

	// 慣性のために減少方式
	velocity *= INERTIA_RATE;
	isMove = false;
}

/*
 *	速度ベクトルをセット
 */
void MoveComponent::SetVelocity(Vector3 direction, float speed) {
	velocity = direction * speed;
	isMove = true;
}
