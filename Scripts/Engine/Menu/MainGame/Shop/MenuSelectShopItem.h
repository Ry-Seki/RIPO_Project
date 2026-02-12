/*
 *	@file	MenuSelectShopItem.h
 *	@author	Seki
 */

#ifndef MENU_SELECT_SHOP_ITEM_H_
#define MENU_SELECT_SHOP_ITEM_H_

#include "../../MenuBase.h"
#include "../../../UI/Button/UIButtonBase.h"
#include "../../../UI/Sprite/Sprite.h"
#include "../../../UI/EventSystem.h"
#include "../../../GameEnum.h"
#include "../../../../Data/ItemCatalogData.h"

#include <vector>
#include <memory>
#include <functional>

 // 前方宣言
class Engine;

/*
 *	@brief	購入するアイテムを選択するメニュー
 */
class MenuSelectShopItem : public MenuBase {
private:
	int animFrame = 0;
	float animTimer = 0.0f;
	int selectItemID = -1;
	int currentMoney = -1;
	ItemCatalogData catalogData;
	EventSystem eventSystem;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::function<void()> Callback = nullptr;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Shop/SelectItem/SelectItemMenuResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/Shop/SelectItem/SelectItemMenuNavigation.json";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuSelectShopItem() override {}

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
	 *	@brief	アニメーション等の更新
	 */
	void AnimUpdate(Engine& engine, float unscaledDeltaTime) override;
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
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void()> setCallback
	 */
	inline void SetCallback(std::function<void()> setCallback) {
		Callback = setCallback;
	}
};

#endif // !MENU_SELECT_SHOP_ITEM_H_