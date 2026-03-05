/*
 *	@file	OBBCollider.h
 *	@author Riku
 */

#ifndef _OBBCOLLIDER_H_
#define _OBBCOLLIDER_H_

#include "../Collision/ColliderBase.h"

/*
 *	OBBコライダー
 */
class OBBCollider : public ColliderBase {
public:
	OBB obb;	// 衝突判定

public:
	OBBCollider() = default;
	virtual ~OBBCollider() = default;

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
using OBBColliderPtr = std::shared_ptr<OBBCollider>;
using OBBColliderList = std::vector<OBBColliderPtr>;

#endif // !_OBBCOLLIDER_H_
