/*
 *	@file	MenuSelectSaveSlot.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_SAVE_SLOT_H_
#define _MENU_SELECT_SAVE_SLOT_H_

#include "../MenuBase.h"
#include "../../UI/EventSystem.h"

#include <vector>
#include <memory>

// 前方宣言
class UIButton;

/*
 *	@brief	セーブスロットを選択するメニュー
 */
class MenuSelectSaveSlot : public MenuBase {
private:
	std::vector<std::shared_ptr<UIButton>> buttonList;
	EventSystem eventSystem;

	static constexpr const char* _BUTTON_IMAGE_PATH = "Res/Menu/System/Trealine_SaveSlot.png";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/System/SaveSlotNavigation.json";

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize () override;
	/*
	 *	@brief	メニューを開く
	 */
	void Open () override;
	/*
	 *	@brief	更新処理
	 */
	void Update (Engine& engine, float deltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render () override;
	/*
	 *	@brief	メニューを閉じる
	 */
	void Close () override;

public:
};

#endif // !_MENU_SELECT_SAVE_SLOT_H_