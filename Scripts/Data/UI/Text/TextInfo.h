/*
 *	@file	TextInfo.h
 *	@author	Seki
 */

#ifndef _TEXT_INFO_H_
#define _TEXT_INFO_H_

#include "../../../Engine/GameEnum.h"

#include <string>
#include <vector>

/*
 *	@brief	テキスト情報構造体
 */
struct TextInfo {
	int x = -1, y = -1;
	int width = -1;
	int size = -1;
	std::string name = "";
	std::string useFont = "";
	GameEnum::TextType textType = GameEnum::TextType::Invalid;
	GameEnum::AlignmentType alignment = GameEnum::AlignmentType::Invalid;
	std::vector<std::string> textData;
};

#endif // !_TEXT_INFO_H_