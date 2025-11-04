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
	bool isHit = false;			// 当たっているかどうか
	AABB aabb;					// 衝突判定

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
	void DebugRender();
};
// 別名定義
using AABBColliderPtr = std::shared_ptr<AABBCollider>;

#endif // !_AABBCOLLIDER_H_
