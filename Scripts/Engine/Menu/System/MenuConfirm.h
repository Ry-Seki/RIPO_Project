/*
 *	@file	MenuConfirm.h
 *	@author	Seki
 */

#ifndef _MENU_DIALOG_H_
#define _MENU_DIALOG_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"
#include "../../GameConst.h"

#include <functional>

// 前方宣言
class Engine;

/*
 *	@brief	確認メニュー
 */
class MenuConfirm : public MenuBase {
private:
	int currentSlot = -1;
	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::function<void(GameEnum::ConfirmResult)> Callback = nullptr;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/System/Confirm/ConfirmResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/System/Confirm/ConfirmNavigation.json";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuConfirm() override {
	}

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
	/*
	 *	@brief	メニューを中断
	 */
	void Suspend() override;
	/*
	 *	@brief	メニューを再開
	 */
	void Resume() override;

private:
	/*
	 *	@brief		ボタンの押された時の処理
	 *	@param[in]	int buttonIndex
	 */
	void SelectButtonExecute(Engine& engine, int buttonIndex);

public:
	/*
	 *	@brief		コールバックの登録
	 *	@param[in]	std::function<void()> setCallback
	 */
	inline void SetCallback(std::function<void(GameEnum::ConfirmResult)> setCallback) {
		if (!setCallback) return;

		Callback = setCallback;
	}
};

#endif // !_MENU_DIALOG_H_