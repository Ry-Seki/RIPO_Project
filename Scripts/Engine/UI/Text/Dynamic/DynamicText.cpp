/*
 *	@file	DynamicText.cpp
 *	@author	Seki
 */

#include "DynamicText.h"

#include "../../../Manager/FontManager.h"

#include <DxLib.h>

/*
 *	@brief	描画処理
 */
void DynamicText::Render() {
    auto& font = FontManager::GetInstance();
    
    int drawX = CalcAlignmentX(width);
    int drawY = textInfo.y;

    font.Draw(
        textInfo.useFont,
        drawX,
        drawY,
        currentText,
        color
    );
}
/*
 *	@brief	文字の横幅の計算
 */
void DynamicText::CalcTextWidth() {
    auto& font = FontManager::GetInstance();
    int fontHandle = font.GetHandle(textInfo.useFont);

    width = GetDrawStringWidthToHandle(
        currentText.c_str(),
        currentText.size(),
        fontHandle
    );
}
/*
 *	@brief		テキストの設定
 *	@param[in]	const std::string& setText
 */
void DynamicText::SetText(const std::string& setText) {
	currentText = setText;
	// 横幅の背適応
	CalcTextWidth();
}
