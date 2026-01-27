/*
 *	@file	MenuActionMap.h
 *	@author	Seki
 */

#ifndef _MENU_ACTION_MAP_H_
#define _MENU_ACTION_MAP_H_

#include "ActionMapBase.h"

/*
 *	@brief	メニュー用入力受付
 */
class MenuActionMap : public ActionMapBase {
private:

public:
	/*
	 *	@brief	コンストラクタ
	 */
	MenuActionMap() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~MenuActionMap() override = default;

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize() override;
};

#endif // !_MENU_ACTION_MAP_H_