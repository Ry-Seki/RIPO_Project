/*
 *	@file	CapsuleCollider.h
 *  @author Riku
 */

#ifndef _CAPSULECOLLIDER_H_
#define _CAPSULECOLLIDER_H_

#include "../Collision/ColliderBase.h"

/*
 *	カプセルコライダー
 */
class CapsuleCollider : public ColliderBase {
public:
	Capsule capsule;	// 衝突判定

public:
	CapsuleCollider() = default;
	virtual ~CapsuleCollider() = default;

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
	 *	デバッグ描画関数
	 */
	void DebugRender() override;

};

#endif // !_CAPSULECOLLIDER_H_
