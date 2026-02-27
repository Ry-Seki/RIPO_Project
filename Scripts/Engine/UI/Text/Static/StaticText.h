/*
 *	@brief	StaticText.h
 *	@author	Seki
 */

#ifndef _STATIC_TEXT_H_
#define _STATIC_TEXT_H_

#include "../TextBase.h"

#include <vector>

/*
 *	@brief	外部データの文字列を描画するテキストクラス
 */
class StaticText : public TextBase {
private:
	std::vector<int> lineWidthList;

public:
	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	const TextInfo& setInfo
	 */
	StaticText(const TextInfo& setInfo)
		: TextBase(setInfo) {
	}
	/*
	 *	@brief	デストラクタ
	 */
	~StaticText() override = default;

public:
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	文字の横幅の計算
	 */
	void CalcTextWidth() override;
};

#endif // !_STATIC_TEXT_H_