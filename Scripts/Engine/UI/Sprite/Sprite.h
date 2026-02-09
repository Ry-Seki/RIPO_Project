/*
 *	@file	Sprite.h
 *	@author	Seki
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "../Rect.h"

#include <vector>

/*
 *	@brief	スプライトクラス
 */
class Sprite {
private:
	int currentIndex = -1;
	std::vector<int> graphHandleList;
	Rect rect;

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize() {}
	/*
	 *	@brief	準備前処理
	 */
	void Setup();
	/*
	 *	@brief	描画処理
	 */
	void Render();

public:
	/*
	 *	@brief		現在の画像番号の設定
	 *	@return		int
	 */
	inline int GetFrameCount() const {
		return static_cast<int>(graphHandleList.size());
	}
	/*
	 *	@brief		現在の画像番号を設定
	 *	@param[in]	int index
	 */
	inline void SetFrameIndex(int index) {
		if (index < 0 || index >= graphHandleList.size()) return;
		currentIndex = index;
	}
	/*
	 *	@brief		画像のサイズ設定
	 *	@param[in]	const Rect& setRect
	 */
	inline void SetRect(const Rect& setRect) { rect = setRect; }
	/*
	 *	@brief		画像ハンドルの追加
	 *	@param[in]	int setHandle
	 */
	inline void SetGraphHandle(int setHandle) { graphHandleList.push_back(setHandle); }
};

#endif // !_SPRITE_H_