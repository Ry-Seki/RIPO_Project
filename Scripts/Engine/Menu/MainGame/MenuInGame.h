/*
 *	@file	MenuInGame.h
 *	@author	Seki
 */

#ifndef _MENU_IN_GAME_H_
#define _MENU_IN_GAME_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"

#include <vector>
#include <memory>

/*
 *	@brief	インゲームメニュー
 */
class MenuInGame : public MenuBase {
private:
	int currentSlot = -1;
	GameEnum::SaveSlotMenuMode saveMode;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	EventSystem eventSystem;

	const int _BUTTON_INDEX = 4;
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/System/SaveLoad/SaveSlotNavigation.json";

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize() override;
	/*
	 *	@brief	メニューを開く
	 */
	void Open() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	メニューを閉じる
	 */
	void Close() override;
	/*
	 *	@brief	メニューを中断
	 */
	void Suspend() override;
	/*
	 *	@brief	メニューを再開
	 */
	void Resume() override;
};

#endif // !_MENU_IN_GAME_H_