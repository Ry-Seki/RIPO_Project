/*
 *	@file	SelectDetail_Shop.cpp
 *	@author	Seki
 */

#include "SelectDetail_Shop.h"
#include "../GameState_SelectDetail.h"
#include "../../GameStateMachine.h"
#include "../../../../System/Money/MoneyManager.h"
#include "../../../../System/Item/ItemManager.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Audio/AudioUtility.h"
#include "../../../../Menu/MenuManager.h"
#include "../../../../Menu/MainGame/Shop/MenuSelectShopItem.h"
#include "../../../../Menu/MainGame/Shop/MenuPurchaseCount.h"

/*
 *	@brief	初期化処理
 */
void SelectDetail_Shop::Initialize() {
	auto& menu = MenuManager::GetInstance();
	auto select = menu.GetMenu<MenuSelectShopItem>();
	auto purchase = menu.GetMenu<MenuPurchaseCount>();

	select->SetCallback([this]() {
		owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
	});
	purchase->SetCallBack([this](int itemID, int buyCount) {
		BuyItem(itemID, buyCount);
	});
}
/*
 *	@brief	準備前処理
 */
void SelectDetail_Shop::Setup() {
	MenuManager::GetInstance().OpenMenu<MenuSelectShopItem>();
}
/*
 *	@brief	更新処理
 */
void SelectDetail_Shop::Update(float deltaTime) {
	if (!isStart) return;

}
/*
 *	@brief	描画処理
 */
void SelectDetail_Shop::Render() {
	if (!isStart) return;
}
/*
 *	@brief	片付け処理
 */
void SelectDetail_Shop::Teardown() {
}
/*
 *	@brief	購入処理
 *	@param[in]	int itemID
 *	@param[in]	int buyCount
 */
void SelectDetail_Shop::BuyItem(int itemID, int buyCount) {
	auto& context = owner->GetOwner()->GetActionContext();
	for (int i = 0; i < buyCount; i++) {
		context.buyIDList.push_back(itemID);
	}
}
