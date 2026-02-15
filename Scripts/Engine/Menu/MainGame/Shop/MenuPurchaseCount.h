/*
 *	@file	MenuPurchaseCount.h
 *	@author	Seki
 */

#ifndef _MENU_PURCHASE_COUNT_H_
#define _MENU_PURCHASE_COUNT_H_

#include "../../MenuBase.h"
#include "../../../UI/Button/UIButtonBase.h"
#include "../../../UI/Sprite/Sprite.h"
#include "../../../UI/EventSystem.h"
#include "../../../GameEnum.h"

#include <vector>
#include <memory>
#include <functional>

 // 前方宣言
class Engine;
struct ItemData;

/*
 *	@brief	アイテムの購入数を調整するメニュー
 */
class MenuPurchaseCount : public MenuBase {
private:
	enum class ButtonType {
		Invalid = -1,
		AddButton,
		SubButton,
		BuyButton,
		CancelButton,
		Max
	};

private:
	int buyButtonIndex = -1;
	int purchaseCount = -1;
	int currentMoney = -1;
	int purchaseMoney = -1;
	int leastMoney = -1;
	int animFrame = 0;
	float animTimer = 0.0f;
	ItemData* targetItemData = nullptr;
	EventSystem eventSystem;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::function<void(int, int)> Callback = nullptr;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Shop/PurchaseCount/PurchaseMenuResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/Shop/PurchaseCount/PurchaseMenuNavigation.json";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuPurchaseCount() override {}

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
	/*
	 *	@brief		購入の確定
	 */
	void ConfirmPurchaseItem();
	/*
	 *	@brief		アイテムの購入個数を一つ増加
	 */
	void AddPurchaseCount();
	/*
	 *	@brief		アイテムの購入個数を一つ減少
	 */
	void SubPurchaseCount();

public:
	/*
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void(int, int)> setCallBack
	 */
	inline void SetCallBack(std::function<void(int, int)> setCallback) {
		Callback = setCallback;
	}
	/*
	 *	@brief		アイテムデータの設定
	 *	@param[in]	ItemData* setData
	 */
	inline void SetItemData(ItemData* setData) {
		targetItemData = setData;
	}
};

#endif // !_MENU_PURCHASE_COUNT_H_