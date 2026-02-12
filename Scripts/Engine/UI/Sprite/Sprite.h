/*
 *	@file	Sprite.h
 *	@author	Seki
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "../Rect.h"

#include <vector>
#include <string>
/*
 *	@brief	スプライトクラス
 */
class Sprite {
private:
	int currentIndex = -1;
	bool isVisible = false;
	std::vector<int> graphHandleList;
	Rect rect;
	std::string name = "";

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
	 *	@brief		描画判定
	 *	@return		bool
	 */
	inline bool IsVisible() const { return isVisible; }
	/*
	 *	@brief		描画判定の設定
	 *	@param[in]	bool setFlag
	 */
	inline void SetIsVisible(bool setFlag) { isVisible = setFlag; }
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
	/*
	 *	@brief		名前の取得
	 *	@return		std::string
	 */
	inline std::string GetName() const { return name; }
	/*
	 *	@brief		名前の設定
	 *	@param[in]	const std::string& setName
	 */
	inline void SetName(const std::string& setName) { name = setName; }
};

#endif // !_SPRITE_H_