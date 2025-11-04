/*
 *	@file	AABBCollider.h
 *  @author Riku
 */

#ifndef _AABBCOLLIDER_H_
#define _AABBCOLLIDER_H_

#include "Component.h"
#include "../Collision.h"
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
	void Update(float deltaTime) override;
};
// 別名定義
using AABBColliderPtr = std::shared_ptr<AABBCollider>;

#endif // !_AABBCOLLIDER_H_
