/*
 *	@file	Text.h
 *	@author	Seki
 */

#ifndef _TEXT_H_
#define _TEXT_H_

#include "../../../Data/UI/Text/TextInfo.h"

/*
 *	@brief	テキストクラス
 */
class Text {
private:
	TextInfo textInfo;

public:
	/*
	 *	@brief		コンストラクタ
	 *	
	 */
	Text(const TextInfo& setInfo)
		: textInfo(setInfo){
	}

public:
	/*
	 *	@brief	描画処理
	 */
	void Draw();

};

#endif // !_TEXT_H_