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

/*
 *	@brief	初期化処理
 */
void SelectDetail_Shop::Initialize() {
}
/*
 *	@brief	準備前処理
 */
void SelectDetail_Shop::Setup() {
	isStart = false;
	inputHandle = false;
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		isStart = true;
		MoneyManager::GetInstance().SetItemDataList(itemDataList);
	});
}
/*
 *	@brief	更新処理
 */
void SelectDetail_Shop::Update(float deltaTime) {
	if (!isStart) return;

	MoneyManager& money = MoneyManager::GetInstance();
	if (!inputHandle) {
		auto& context = owner->GetOwner()->GetActionContext();
		if (CheckHitKey(KEY_INPUT_1)) {
			inputHandle = true;
			int ID = 0;
			if (money.GetCurrentMoney() < itemDataList[ID]->price) return;

			// SEの再生
			AudioUtility::PlaySE("DebugSE");
			// 所持金から引く
			money.SubItemMoney(ID);
			// 購入済みリストに追加
			buyItemIDList.push_back(ID);
		} else if (CheckHitKey(KEY_INPUT_2)) {
			inputHandle = true;
			int ID = 1;
			if (money.GetCurrentMoney() < itemDataList[ID]->price) return;

			// SEの再生
			AudioUtility::PlaySE("DebugSE");
			// 所持金から引く
			money.SubItemMoney(ID);
			// 購入済みリストに追加
			buyItemIDList.push_back(ID);
		} else if (CheckHitKey(KEY_INPUT_3)) {
			inputHandle = true;
			int ID = 2;
			if (money.GetCurrentMoney() < itemDataList[ID]->price) return;

			// SEの再生
			AudioUtility::PlaySE("DebugSE");
			// 所持金から引く
			money.SubItemMoney(ID);
			// 購入済みリストに追加
			buyItemIDList.push_back(ID);
		} else if (CheckHitKey(KEY_INPUT_4)) {
			inputHandle = true;
			int ID = 3;
			if (money.GetCurrentMoney() < itemDataList[ID]->price) return;

			// SEの再生
			AudioUtility::PlaySE("DebugSE");
			// 所持金から引く
			money.SubItemMoney(ID);
			// 購入済みリストに追加
			buyItemIDList.push_back(ID);
		} else if (CheckHitKey(KEY_INPUT_SPACE)) {
			// SEの再生
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			isStart = false;
			context.buyIDList = buyItemIDList;
			FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
			FadeManager::GetInstance().StartFade(fade, [this]() {
				owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
			});
		}

	}
	// キー離しで再度入力受付
	if (CheckHitKey(KEY_INPUT_1) == 0 && CheckHitKey(KEY_INPUT_2) == 0 
		&& CheckHitKey(KEY_INPUT_3) == 0 && CheckHitKey(KEY_INPUT_4) == 0) {
		inputHandle = false;
	}

}
/*
 *	@brief	描画処理
 */
void SelectDetail_Shop::Render() {
	if (!isStart) return;

	DrawFormatString(50, 50, GetColor(255, 255, 255), "=== Selection Shop Sample ===");
	DrawFormatString(50, 80, GetColor(0, 255, 0), "1: HP");
	DrawFormatString(50, 100, GetColor(0, 255, 0), "2: Stamina");
	DrawFormatString(50, 120, GetColor(0, 255, 0), "3: Strength");
	DrawFormatString(50, 140, GetColor(0, 255, 0), "4: ResistTime");
	DrawFormatString(50, 160, GetColor(0, 255, 0), "BuyItemList : ");

	auto& context = owner->GetOwner()->GetActionContext();
	for (int i = 0, max = context.buyIDList.size(); i < max; i++) {
		DrawFormatString(startX + i * intervalX, 160, GetColor(255, 255, 255), "%d", context.buyIDList[i] + 1);
	}
}
/*
 *	@brief	片付け処理
 */
void SelectDetail_Shop::Teardown() {
}
