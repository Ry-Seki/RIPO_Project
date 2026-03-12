/*
 *	@file	MenuSelectShopItem.h
 *	@author	Seki
 */

#ifndef MENU_SELECT_SHOP_ITEM_H_
#define MENU_SELECT_SHOP_ITEM_H_

#include "../../MenuBase.h"
#include "../../../UI/Button/UIButtonBase.h"
#include "../../../UI/Sprite/Sprite.h"
#include "../../../UI/Text/TextBase.h"
#include "../../../UI/EventSystem.h"
#include "../../../GameEnum.h"
#include "../../../../Data/ItemCatalogData.h"

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
 // 前方宣言
class Engine;

/*
 *	@brief	購入するアイテムを選択するメニュー
 */
class MenuSelectShopItem : public MenuBase {
private:
	int currentSlot = -1;
	int selectItemID = -1;
	int currentMoney = -1;
	ItemCatalogData catalogData;
	EventSystem eventSystem;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::vector<std::shared_ptr<TextBase>> textList;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;

	std::unordered_map<std::string, UIButtonBase*> buttonMap;

	std::function<void(GameEnum::ShopActionType)> Callback = nullptr;
	std::function<bool()> IsBuyItem = nullptr;

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
	 *	@brief	メニューを再開
	 */
	void Resume() override;

private:
	/*
	 *	@brief		ボタンの押された時の処理
	 *	@param[in]	GameEnum::ShopActionType type
	 *  @param[in]  int itemID
	 */
	void SelectButtonExecute(GameEnum::ShopActionType type, int itemID);
	/*
	 *	@brief		確認メニューを開く
	 *	@param[in]	GameEnum::ShopActionType type
	 */
	void OpenConfirmMenu(GameEnum::ShopActionType type);
	/*
	 *	@brief		ショップボタンの準備前処理
	 *	@param[in]	const JSON& json
	 */
	void SetupShopButtons(const JSON& json);
	/*
	 *	@brief		ショップボタンの更新処理
	 */
	void UpdateButtonState();
	/*
	 *	@brief		名前でのボタン検索
	 *	@param[in]	const std::string& buttonName
	 *	@return		UIButtonBase*
	 */
	UIButtonBase* FindButtonByName(const std::string& buttonName);

public:
	/*
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void(GameEnum::ShopActionType)> setCallback
	 */
	inline void SetCallback(std::function<void(GameEnum::ShopActionType)> setCallback) {
		Callback = setCallback;
	}
	/*
	 *	@brief		アイテムの購入判定関数の設定
	 *	@param[in]	std::function<bool()> setIsBuyItem
	 */
	inline void SetIsBuyItem(std::function<bool()> setIsBuyItem) {
		IsBuyItem = setIsBuyItem;
	}
};

#endif // !MENU_SELECT_SHOP_ITEM_H_