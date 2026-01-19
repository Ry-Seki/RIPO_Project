/*
 *	@file	MiniGameBase.h
 *	@author	Seki
 */

#ifndef _MINI_GAME_BASE_H_
#define _MINI_GAME_BASE_H_

#include <memory>

// 前方宣言
class Engine;

/*
 *	@brief	アルバイト時のミニゲーム基底クラス
 */
class MiniGameBase {
protected:
	int score = 0;
	bool isComplete = false;

public:
	/*
	 *	@brief	初期化処理
	 */
	virtual void Initialize() {}
	/*
	 *	@brief	ミニゲーム開始時処理
	 */
	virtual void Open() {
		score = 0;
		isComplete = false;
	}
	/*
	 *	@brief	更新処理
	 */
	virtual void Update(float deltaTime) = 0;
	/*
	 *	@brief	描画処理
	 */
	virtual void Render() = 0;
	/*
	 *	@brief	ミニゲーム終了時処理
	 */
	virtual void Close() { isComplete = true; }

public:
	/*
	 *	@brief		スコアの取得
	 *	@return		int
	 */
	inline int GetScore() const { return score; }
	/*
	 *	@brief		ミニゲーム完了フラグの取得
	 *	@return		bool
	 */
	inline bool IsComplete() const { return isComplete; }
};
// 別名定義
using MiniGameBasePtr = std::shared_ptr<MiniGameBase>;

#endif // !_MINI_GAME_BASE_H_