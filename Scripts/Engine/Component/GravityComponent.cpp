/*
 *	@file	GravityComponent.cpp
 *  @author Riku
 */

#include "GravityComponent.h"
#include "../GameObject.h"

GravityComponent::GravityComponent() 
	: fallSpeed(0.0f)
	
	, FALL_SPEED_MAX(3000.0f)
	, GRAVITY_ACCELERATION(3000.0f)
{}

void GravityComponent::Update(float deltaTime) {
	GameObject* owner = GetOwner();
	if (!owner) return;

	// —Ž‰º‘¬“xŒvŽZ
	fallSpeed += GRAVITY_ACCELERATION * deltaTime;
	if (fallSpeed >= FALL_SPEED_MAX)
		fallSpeed = FALL_SPEED_MAX;

	// —Ž‰º
	owner->position.y -= fallSpeed * deltaTime;
}