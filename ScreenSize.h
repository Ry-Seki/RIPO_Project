/*
 *	@file	ScreenSize.h
 *	@author	Seki
 */

#ifndef _SCREEN_SIZE_H_
#define _SCREEN_SIZE_H_

#include <DxLib.h>

/*
 *  @brief  画面サイズ構造体
 */
struct ScreenSize {
    int width;
    int height;
};
/*
 *  @brief  スクリーンサイズ取得
 *  @return ScreenSize
 */
inline ScreenSize GetScreenSize() {
    ScreenSize s{};
    GetDrawScreenSize(&s.width, &s.height);
    return s;
}

#endif // !_SCREEN_SIZE_H_