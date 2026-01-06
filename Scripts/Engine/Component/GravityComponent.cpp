/*
 *	@file	GravityComponent.cpp
 *  @author Riku
 */

#include "GravityComponent.h"
#include "../GameObject.h"

GravityComponent::GravityComponent()
	: fallSpeed(0.0f)
	, hitGroundingFrag(false)
	, isGravity(false)
	, isFirstFrame(true)
	, FALL_SPEED_MAX(3000.0f)
	, GRAVITY_ACCELERATION(3000.0f) {
}

void GravityComponent::Update(float deltaTime) {
	GameObject* owner = GetOwner();
	if (!owner) return;

	// 重力がかかるまで処理しない
	if (!isGravity) {
		fallSpeed = 0.0f;
		return;
	}

	// 接地していた場合は落下しない
	if (hitGroundingFrag) {
		// 落下速度を0にする
		fallSpeed = 0.0f;
		return;
	}


	// 落下速度計算
	fallSpeed += GRAVITY_ACCELERATION * deltaTime;
	if (fallSpeed >= FALL_SPEED_MAX)
		fallSpeed = FALL_SPEED_MAX;

	owner->position.y -= fallSpeed * deltaTime;
	isFirstFrame = false;
}