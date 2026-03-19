/*
 *	@file	MenuPurchaseCount.h
 *	@author	Seki
 */

#ifndef _MENU_PURCHASE_COUNT_H_
#define _MENU_PURCHASE_COUNT_H_

#include "../../MenuBase.h"
#include "../../../UI/Button/UIButtonBase.h"
#include "../../../UI/Text/TextBase.h"
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
public:
	/*
	 *	@brief	購入ボタンの種類
	 */
	enum class PurchaseButtonType {
		Invalid = -1,
		AddButton,
		SubButton,
		BuyButton,
		CancelButton,
		Max
	};

private:
	int purchaseCount = -1;
	int currentMoney = -1;
	int purchaseMoney = -1;
	int leastMoney = -1;
	ItemData* targetItemData = nullptr;
	EventSystem eventSystem;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::vector<std::shared_ptr<TextBase>> textList;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::unordered_map<std::string, UIButtonBase*> buttonMap;
	std::vector<std::string> purchaseList;
	std::function<void(int, int)> Callback = nullptr;

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
	 *	@brief	メニューを再開
	 */
	void Resume() override;

private:
	/*
	 *	@brief		ボタンの押された時の処理
	 *	@param[in]	PurchaseButtonType type
	 */
	void SelectButtonExecute(MenuPurchaseCount::PurchaseButtonType type);
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
	/*
	 *	@brief		購入ボタンの準備前処理
	 *	@param[in]	const JSON& json
	 */
	void SetupPurchaseButtons(const JSON& json);
	/*
	 *	@brief		名前でのボタン検索
	 *	@param[in]	const std::string& buttonName
	 *	@return		UIButtonBase*
	 */
	UIButtonBase* FindButtonByName(const std::string& buttonName);

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