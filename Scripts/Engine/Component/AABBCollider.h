/*
 *	@file	AABBCollider.h
 *  @author Riku
 */

#ifndef _AABBCOLLIDER_H_
#define _AABBCOLLIDER_H_

#include "../Collision/ColliderBase.h"

 /*
  *	AABBコライダー
  */
class AABBCollider : public ColliderBase {
public:
	AABB aabb;	// 衝突判定

public:
	AABBCollider() = default;
	virtual ~AABBCollider() = default;

public:
	/*
	 *	更新処理
	 */
	void Update(float deltaTime) override;
	/*
	 *	衝突が起きたときに呼び出される処理
	 *	@param	const std::shared_ptr<Component&> self	衝突した自分のコライダー
	 *  @param	const std::shared_ptr<Component&> other	衝突した相手のコライダー
	 */
	void OnCollision(
		const std::shared_ptr<Component>& self,
		const std::shared_ptr<Component>& other) override;
public:
	/*
	 *	デバッグ用の描画関数
	 */
	void DebugRender() override;
};
// 別名定義
using AABBColliderPtr = std::shared_ptr<AABBCollider>;
using AABBColliderList = std::vector<AABBColliderPtr>;

#endif // !_AABBCOLLIDER_H_
