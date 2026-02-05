/*
 *	@file	MenuCredit.h
 *	@author	Seki
 */

#ifndef _MENU_CREDIT_H_
#define _MENU_CREDIT_H_

#include "../MenuBase.h"

// 前方宣言
class Engine;

/*
 *	@brief	クレジットメニュー
 */
class MenuCredit : public MenuBase {
private:

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/System/Confirm/ConfirmResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/System/Confirm/ConfirmNavigation.json";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuCredit() override {};

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

};

#endif // !_MENU_CREDIT_H_