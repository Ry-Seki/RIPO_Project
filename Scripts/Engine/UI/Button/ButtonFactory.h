/*
 *	@file	ButtonFactory.h
 *	@author	Seki
 */

#ifndef _BUTTON_FACTORY_H_
#define _BUTTON_FACTORY_H_

#include <vector>
#include <memory>

// 前方宣言
class UIButtonBase;
struct ButtonInfo;

/*
 *	@brief	ボタン生成クラス
 */
class ButtonFactory {
public:
	/*
	 *	@brief		ボタンを生成
	 *  @param[in]	const ButtonInfo& buttonInfo
	 *	@return		std::shared_ptr<UIButtonBase>
	 */
	static std::shared_ptr<UIButtonBase> CreateButton(const ButtonInfo& buttonInfo);
};

#endif // !_BUTTON_FACTORY_H_