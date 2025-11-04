/*
 *	@file	AABBCollider.cpp
 *  @author Riku
 */

#include "AABBCollider.h"
#include "../GameObject.h"

void AABBCollider::Update(float deltaTime) {
	GameObject* owner = GetOwner();
	VECTOR aabbMin = ToVECTOR(aabb.min + owner->position);
	VECTOR aabbMax = ToVECTOR(aabb.max + owner->position);
	std::vector<VECTOR> aabbPoits = {
		aabbMin,
		{ aabbMin.x, aabbMin.y, aabbMax.z },
		{ aabbMax.x, aabbMin.y, aabbMin.z },
		{ aabbMax.x, aabbMin.y, aabbMax.z },
		{ aabbMin.x, aabbMax.y, aabbMax.z },
		aabbMax,
		{ aabbMax.x, aabbMax.y, aabbMin.z },
		{ aabbMin.x, aabbMax.y, aabbMin.z }
	};

	DrawLine3D(aabbPoits[0], aabbPoits[1], GetColor(255, 255, 255));
	DrawLine3D(aabbPoits[0], aabbPoits[2], GetColor(255, 255, 255));
	DrawLine3D(aabbPoits[0], aabbPoits[7], GetColor(255, 255, 255));
	DrawLine3D(aabbPoits[7], aabbPoits[5], GetColor(255, 255, 255));
	DrawLine3D(aabbPoits[7], aabbPoits[6], GetColor(255, 255, 255));
	DrawLine3D(aabbPoits[4], aabbPoits[5], GetColor(255, 255, 255));
	DrawLine3D(aabbPoits[4], aabbPoits[6], GetColor(255, 255, 255));
	DrawLine3D(aabbPoits[4], aabbPoits[3], GetColor(255, 255, 255));
	DrawLine3D(aabbPoits[3], aabbPoits[1], GetColor(255, 255, 255));
	DrawLine3D(aabbPoits[3], aabbPoits[2], GetColor(255, 255, 255));

}
