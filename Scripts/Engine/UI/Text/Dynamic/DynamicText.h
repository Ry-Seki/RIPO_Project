/*
 *	@file	DynamicText.h
 *	@author	Seki
 */

#ifndef _DYNAMIC_TEXT_H_
#define _DYNAMIC_TEXT_H_

#include "../TextBase.h"

#include <vector>

/*
 *	@brief	動的な文字列を描画するテキストクラス(1行のみで完結するもの)
 */
class DynamicText : public TextBase {
private:
	int width = -1;
	std::string currentText = "";

public:
	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	const TextInfo& setInfo
	 */
	DynamicText(const TextInfo& setInfo)
		: TextBase(setInfo) {
	}
	/*
	 *	@brief	デストラクタ
	 */
	~DynamicText() override = default;

public:
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	文字の横幅の計算
	 */
	void CalcTextWidth() override;
	/*
	 *	@brief		テキストの設定
	 *	@param[in]	const std::string& setText
	 */
	void SetText(const std::string& setText) override;

};

#endif // !_DYNAMIC_TEXT_H_