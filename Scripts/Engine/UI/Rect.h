/*
 *	@file	Rect.h
 *	@author	Seki
 */

#ifndef _RECT_H_
#define _RECT_H_

/*
 *	@brief	矩形構造体
 */
struct Rect {
	int x = -1;
	int y = -1;
	int width = -1;
	int height = -1;

	/*
	 *	@brief	コンストラクタ
	 */
	Rect() 
		: x(0), y(0), width(0), height(0){
	}
	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	int x, y
	 *	@param[in]	int width, height
	 */
	Rect(int setX, int setY, int setWidth, int setHeight)
		: x(setX), y(setY), width(setWidth), height(setHeight){
	}
	/*
	 *	@brief		マウスポインタが触れているか判定
	 *	@param[in]	int px, int py
	 *  @return		bool
	 */
	bool IsHovered (int px, int py) const {
		return (px >= x && px < x + width &&
				py >= y && py < y + height);
	}
};
#endif // !_RECT_H_