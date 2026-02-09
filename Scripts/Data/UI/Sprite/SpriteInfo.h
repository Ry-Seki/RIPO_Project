/*
 *	@file	SpriteInfo.h
 *	@author	Seki
 */

#ifndef _SPRITE_INFO_H_
#define _SPRITE_INFO_H_

#include "../../../Engine/UI/Rect.h"

#include <string>
#include <vector>

/*
 *	@brief	スプライト情報構造体
 */
struct SpriteInfo {
    std::string name;
    Rect rect;
    std::vector<std::string> resourcePathList;
};

#endif // !_SPRITE_INFO_H_