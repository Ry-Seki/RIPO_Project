/*
 *	@file	MenuSelectNewGame.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_NEW_GAME_H_
#define _MENU_SELECT_NEW_GAME_H_

#include "../MenuBase.h"
#include "../../UI/EventSystem.h"

#include <vector>
#include <memory>

// 前方宣言
class Engine;
class SinglePressButton;

/*
 *	@brief	ニューゲームメニュー
 */
class MenuSelectNewGame : public MenuBase {
private:
	std::vector<std::shared_ptr<SinglePressButton>> buttonList;
	EventSystem eventSystem;

	static constexpr const char* _NAVIGATION_PATH = "Data/UI/Title/SaveSlotNavigation.json";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuSelectNewGame() override {};

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

};

#endif // !_MENU_SELECT_NEW_GAME_H_
