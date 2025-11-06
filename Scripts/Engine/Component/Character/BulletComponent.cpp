/*
 *	@file	BulletComponent.cpp
 *  @author	Riku
 */

#include "BulletComponent.h"
#include "../../GameObject.h"

BulletComponent::BulletComponent() 
	: destroyTimeCount(3)

	, SHOT_SPEED(10000)
{}

void BulletComponent::Update(float deltaTime) {
	GameObject* bullet = GetOwner();

	// 前に進む
	Vector3 forward = ForwardDir(bullet->rotation);
	bullet->position = forward * SHOT_SPEED * deltaTime;
	
	// 破棄カウントダウン
	destroyTimeCount -= deltaTime;
	//if (destroyTimeCount < 0) {
	//
	//}
}
