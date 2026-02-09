/*
 *	@file	PlayerUIBase.h
 *	@author	Riku
 */

#ifndef _PLAYERUIBASE_H_
#define _PLAYERUIBASE_H_

#include <memory>

/*
 *	プレイヤーUIの基底クラス
 */
class PlayerUIBase {

public:
	PlayerUIBase() = default;
	~PlayerUIBase() = default;

public:
	/*
	 *	初期化処理
	 */
	virtual void Initialize() = 0;
	/*
	 *	更新処理
	 */
	virtual void Update(float unscaledDeltaTime) = 0;
	/*
	 *	描画処理
	 */
	virtual void Render() = 0;

};
// 別名定義
using PlayerUIBasePtr = std::shared_ptr<PlayerUIBase>;

#endif // !_PLAYERUIBASE_H_
