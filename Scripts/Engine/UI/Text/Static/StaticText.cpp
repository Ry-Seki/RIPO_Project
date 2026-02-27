/*
 *	@file	StaticText.cpp
 *	@author	Seki
 */

#include "StaticText.h"
#include "../../../Manager/FontManager.h"

#include <DxLib.h>

/*
 *	@brief	•`‰æˆ—
 */
void StaticText::Render() {
    auto& font = FontManager::GetInstance();

    // •‚ÌŒvZ
    CalcTextWidth();

    int drawY = textInfo.y;
    // ƒtƒHƒ“ƒg‚Ì•`‰æ
    for (int i = 0, max = textInfo.textLineList.size(); i < max; i++) {
        const std::string& text = textInfo.textLineList[i];
        int drawX = CalcAlignmentX(lineWidthList[i]);

        font.Draw(textInfo.useFont, drawX, drawY, text, color);

        drawY += textInfo.lineHeight;
    }
}
/*
 *	@brief	•¶š‚Ì‰¡•‚ÌŒvZ
 */
void StaticText::CalcTextWidth() {
    lineWidthList.clear();

    int fontHandle = FontManager::GetInstance().GetHandle(textInfo.useFont);

    for (const auto& line : textInfo.textLineList) {
        int width = GetDrawStringWidthToHandle(
            line.c_str(),
            line.size(),
            fontHandle
        );

        lineWidthList.push_back(width);
    }
}
