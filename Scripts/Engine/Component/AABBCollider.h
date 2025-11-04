/*
 *	@file	AABBCollider.h
 *  @author Riku
 */

#ifndef _AABBCOLLIDER_H_
#define _AABBCOLLIDER_H_

#include "Component.h"
#include "../Collision.h"
#include "../GameObject.h"
#include <vector>

/*
 *	AABBコライダー
 */
class AABBCollider : public Component {
public:
	bool isTrigger = false;		// 重複するかどうか
	bool isStatic = false;		// 動くかどうか
	AABB aabb;					// 衝突判定

public:
	AABBCollider() = default;
	virtual ~AABBCollider() = default;

public:
	void Update(float deltaTime) override {
		GameObject* owner = GetOwner();
		VECTOR aabbMin = ToVECTOR(aabb.min + owner->position);
		VECTOR aabbMax = ToVECTOR(aabb.max + owner->position);
		std::vector<VECTOR> aabbPoits = {
			aabbMin,
			{ aabbMin.x, aabbMin.y, aabbMax.z },
			{ aabbMax.x, aabbMin.y, aabbMin.z },
			{ aabbMax.x, aabbMin.y, aabbMax.z },
			aabbMax,
			{ aabbMax.x, aabbMax.y, aabbMin.z },
			{ aabbMin.x, aabbMax.y, aabbMax.z },
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
};
// 別名定義
using AABBColliderPtr = std::shared_ptr<AABBCollider>;

#endif // !_AABBCOLLIDER_H_
