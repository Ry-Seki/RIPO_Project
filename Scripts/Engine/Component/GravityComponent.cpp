/*
 *	@file	GravityComponent.cpp
 *  @author Riku
 */

#include "GravityComponent.h"
#include "../GameObject.h"

GravityComponent::GravityComponent() 
	: fallSpeed(0.0f)
	
	, FALL_SPEED_MAX(100.0f)
	, GRAVITY_ACCELERATION(9.8f)
{}

void GravityComponent::Update(float deltaTime) {
	GameObject* owner = GetOwner();
	if (!owner) return;

	// —Ž‰º‘¬“xŒvŽZ
	fallSpeed += GRAVITY_ACCELERATION * deltaTime;
	// —Ž‰º
	owner->position.y -= fallSpeed * deltaTime;
}