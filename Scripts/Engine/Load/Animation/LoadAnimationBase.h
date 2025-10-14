/*
 *	@file	LoadAnimationBase.h
 *	@author	Seki
 */

#ifndef _LOAD_ANIMATION_BASE_H_
#define _LOAD_ANIMATION_BASE_H_

#include <memory>
#include <DxLib.h>

/*
 *	ロード画面アニメーションの基底クラス
 */
class LoadAnimationBase {
public:
	/*
	 *	デストラクタ
	 */
	virtual ~LoadAnimationBase() = default;

public:
	/*
	 *	更新処理
	 */
	virtual void Update(float deltaTime) = 0;
	/*
	 *	描画処理
	 */
	virtual void Render() = 0;
};
// 別名定義
using LoadAnimationPtr = std::shared_ptr<LoadAnimationBase>;

#endif // !_LOAD_ANIMATION_BASE_H_

