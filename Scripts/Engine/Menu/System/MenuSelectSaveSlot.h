/*
 *	@file	MenuSelectSaveSlot.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_SAVE_SLOT_H_
#define _MENU_SELECT_SAVE_SLOT_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"
#include "../../GameConst.h"

#include <vector>
#include <memory>

// 前方宣言
class Engine;

/*
 *	@brief	セーブスロットを選択するメニュー
 */
class MenuSelectSaveSlot : public MenuBase {
private:
	int currentSlot = -1;
	GameEnum::SaveSlotMenuMode saveMode;
	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;

	static constexpr const char* _BUTTON_IMAGE_PATH = "Res/Menu/System/Trealine_SaveSlot.png";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/System/SaveLoad/SaveSlotNavigation.json";

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize (Engine& engine) override;
	/*
	 *	@brief	メニューを開く
	 */
	void Open () override;
	/*
	 *	@brief	更新処理
	 */
	void Update (Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render () override;
	/*
	 *	@brief	メニューを閉じる
	 */
	void Close (Engine& engine) override;

public:
	/*
	 *	@brief		ボタンのコールバック登録
	 *	@param[in]	int slotIndex
	 */
	void SelectButtonExecute(Engine& engine, int slotIndex);

public:
	/*
	 *	@brief		セーブモードの設定
	 *	@param[in]	GameEnum::SaveSlotMenuMode setSaveMode
	 */
	inline void SetSaveMode(GameEnum::SaveSlotMenuMode setSaveMode) {
		saveMode = setSaveMode;
	}
};

#endif // !_MENU_SELECT_SAVE_SLOT_H_