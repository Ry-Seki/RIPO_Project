/*
 *	@file	MenuSystem.h
 *	@author	Seki
 */

#ifndef _MENU_SYSTEM_H_
#define _MENU_SYSTEM_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/EventSystem.h"

// 前方宣言
class Engine;

/*
 *	@brief	システムメニュー
 */
class MenuSystem : public MenuBase {
private:
	int currentIndex = -1;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	EventSystem eventSystem;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/Title/SelectGameMode/System/SystemResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/Title/SelectGameMode/System/SystemNavigation.json";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuSystem() override {};

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
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
	void Close(Engine& engine) override;

private:
	/*
	 *	@brief		ボタンの押された時の処理
	 *	@param[in]	int buttonIndex
	 */
	void SelectButtonExecute(Engine& engine, int buttonIndex);

};

#endif // !_MENU_SYSTEM_H_
