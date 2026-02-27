/*
 *  @file   TextBase.cpp
 *  @author Seki
 */

#include "TextBase.h"

#include <DxLib.h>

/*
  *	@brief		•¶Žš‚Ì‘µ‚¢‚ÌŒvŽZ
  *	@param[in]	int lineWidth
  *	@return		int
  */
int TextBase::CalcAlignmentX(int lineWidth) {
    switch (textInfo.alignment) {
        case GameEnum::AlignmentType::Left:
            return textInfo.x;

        case GameEnum::AlignmentType::Center:
            return textInfo.x - lineWidth / 2;

        case GameEnum::AlignmentType::Right:
            return textInfo.x - lineWidth;

        default:
            return textInfo.x;
    }
}
