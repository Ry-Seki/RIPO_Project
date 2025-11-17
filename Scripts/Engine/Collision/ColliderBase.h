/*
 *	@file	ColliderBase.h
 *  @author Riku
 */

#ifndef _COLLIDERBASE_H_
#define _COLLIDERBASE_H_

#include "../Component/Component.h"
#include "Collision.h"
#include <vector>

/*
 *	コライダーの基底
 */
class ColliderBase : public Component {
public:
	bool isCollider = false;	// 判定を取るかどうか
	bool isTrigger = false;		// 重複するかどうか
	bool isStatic = false;		// 動くかどうか
	bool isHit = false;			// 当たっているかどうか

public:
	ColliderBase() = default;
	virtual ~ColliderBase() = default;

public:
	/*
	 *	デバッグ用の描画関数
	 */
	virtual void DebugRender() {}
};
// 別名定義
using ColliderBasePtr = std::shared_ptr<ColliderBase>;
using ColliderList = std::vector<ColliderBasePtr>;

#endif // !_COLLIDERBASE_H_
