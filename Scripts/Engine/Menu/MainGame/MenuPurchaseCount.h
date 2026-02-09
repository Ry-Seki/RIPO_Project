/*
 *	@file	MenuPurchaseCount.h
 *	@author	Seki
 */

#ifndef _MENU_PURCHASE_COUNT_H_
#define _MENU_PURCHASE_COUNT_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"

#include <vector>
#include <memory>

 // 前方宣言
class Engine;

/*
 *	@brief	アイテムの購入数を調整するメニュー
 */
class MenuPurchaseCount : public MenuBase {
private:
	int currentSlot = -1;
	int purchaseCount = 0;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	EventSystem eventSystem;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/InGame/InGameMenuResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/InGame/InGameMenuNavigation.json";

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
	/*
	 *	@brief		購入個数の設定
	 */
	void SetPurchaseCount();
	/*
	 *	@brief		アイテムの購入個数を一つ増加
	 */
	void AddPurchaseCount();
	/*
	 *	@brief		アイテムの購入個数を一つ減少
	 */
	void SubPurchaseCount();
};

#endif // !_MENU_PURCHASE_COUNT_H_