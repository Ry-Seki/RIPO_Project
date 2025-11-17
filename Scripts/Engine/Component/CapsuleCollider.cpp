#include "CapsuleCollider.h"

/*
 *	更新処理
 */
void CapsuleCollider::Update(float deltaTime) {
	isHit = false;
}

/*
 *	衝突が起きたときに呼び出される処理
 *	@param	const std::shared_ptr<Component&> self	衝突した自分のコライダー
 *  @param	const std::shared_ptr<Component&> other	衝突した相手のコライダー
 */
void CapsuleCollider::OnCollision(
	const std::shared_ptr<Component>& self,
	const std::shared_ptr<Component>& other) {
	isHit = true;
}

/*
 *	デバッグ描画関数
 */
void CapsuleCollider::DebugRender() {

}
