/*
 *	@file	MenuSettings.h
 *	@author	Seki
 */

#ifndef _MENU_SETTINGS_H_
#define _MENU_SETTINGS_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/EventSystem.h"

#include <vector>
#include <memory>

/*
 *	@brief	設定メニュー
 */
class MenuSettings : public MenuBase {
private:
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	EventSystem eventSystem;

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

#endif // !_MENU_SETTINGS_H_