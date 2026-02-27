/*
 *	@file	TextFactory.h
 *	@author	Seki
 */

#ifndef _TEXT_FACTORY_H_
#define _TEXT_FACTORY_H_

#include <memory>

// 前方宣言
struct TextInfo;
class TextBase;

/*
 *	@brief	テキスト生成クラス
 */
class TextFactory {
public:
	/*
	 *	@brief		テキストを生成
	 *	@param[in]	const TextInfo& textInfo
	 *	@return		std::shared_ptr<TextBase>
	 */
	static std::shared_ptr<TextBase> CreateText(const TextInfo& textInfo);
};

#endif // !_TEXT_FACTORY_H_