/*
 *  @file   TextFactory.cpp
 *  @author Seki
 */

#include "TextFactory.h"
#include "TextBase.h"
#include "Static/StaticText.h"
#include "Dynamic/DynamicText.h"
#include "../../../Data/UI/Text/TextInfo.h"

/*
 *	@brief		テキストを生成
 *	@param[in]	const TextInfo& textInfo
 *	@return		std::shared_ptr<TextBase>
 */
std::shared_ptr<TextBase> TextFactory::CreateText(const TextInfo& textInfo) {
    std::shared_ptr<TextBase> text;
    // テキストの生成
	switch (textInfo.textType) {
		case GameEnum::TextType::Static:
			text = std::make_shared<StaticText>(textInfo);
			break;
		case GameEnum::TextType::Dynamic:
			text = std::make_shared<DynamicText>(textInfo);
			break;
		default:
			return nullptr;
	}

    return text;
}
