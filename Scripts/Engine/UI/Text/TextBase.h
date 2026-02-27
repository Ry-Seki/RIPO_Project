/*
 *	@file	TextBase.h
 *	@author	Seki
 */

#ifndef _TEXT_BASE_H_
#define _TEXT_BASE_H_

#include "../../../Data/UI/Text/TextInfo.h"

/*
 *	@brief	テキストクラス
 */
class TextBase {
protected:
	int color = -1;
	TextInfo textInfo;

public:
	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	const TextInfo& setInfo
	 */
	TextBase(const TextInfo& setInfo)
		: textInfo(setInfo){
	}
	/*
	 *	@brief	デストラクタ
	 */
	virtual ~TextBase() {}

public:
	/*
	 *	@brief	準備前処理
	 */
	virtual void Setup() {}
	/*
	 *	@brief	描画処理
	 */
	virtual void Render() = 0;
	/*
	 *	@brief	文字の横幅の計算
	 */
	virtual void CalcTextWidth() {}
	/*
	 *	@brief		テキストの設定(dynamic用)
	 *	@param[in]	const std::string& setText
	 */
	virtual void SetText(const std::string& setText) {}

public:
	/*
	 *	@brief		文字の揃いの計算
	 *	@param[in]	int lineWidth
	 *	@return		int
	 */
	int CalcAlignmentX(int lineWidth);

public:
	/*
	 *	@brief		テキスト情報の取得
	 *	@return		TextInfo
	 */
	inline TextInfo GetTextInfo() const { return textInfo; }
	/*
	 *	@brief		名前の取得
	 *	@return		std::string
	 */
	inline std::string GetName() const { return textInfo.name; }
	/*
	 *	@brief		色の設定
	 *	@param[in]	int color
	 */
	inline void SetColor(int setColor) {
		color = setColor;
	}
};

#endif // !_TEXT_BASE_H_