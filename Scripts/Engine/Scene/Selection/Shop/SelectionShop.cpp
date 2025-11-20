/*
 *	@file	SelectionShop.cpp
 *	@author	Seki
 */

#include "SelectionShop.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Audio/AudioUtility.h"
#include "../../../System/Money/MoneyManager.h"
#include "../../../System/Item/ItemManager.h"

using namespace AudioUtility;

/*
 *	@brief	初期化処理
 */
void SelectionShop::Initialize(Engine& engine) {
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade);
	MoneyManager::GetInstance().SetItemDataList(itemDataList);
}
/*
 *	@brief	ロード済みデータのセット(コールバック)
 */
void SelectionShop::SetupData(Engine& engine) {

}
/*
 *	@brief	更新処理
 */
void SelectionShop::Update(Engine& engine, float deltaTime) {
	MoneyManager& money = MoneyManager::GetInstance();
	if (!inputHandle && CheckHitKey(KEY_INPUT_1)) {
		inputHandle = true;
		int ID = 0;
		if (money.GetCurrentMoney() < itemDataList[ID]->price) return;

		// SEの再生
		PlaySE("DebugSE");
		// 所持金から引く
		money.SubItemMoney(ID);
		// 購入済みリストに追加
		buyItemIDList.push_back(ID);
	}
	else if (!inputHandle && CheckHitKey(KEY_INPUT_2)) {
		inputHandle = true;
		int ID = 1;
		if (money.GetCurrentMoney() < itemDataList[ID]->price) return;

		// SEの再生
		PlaySE("DebugSE");
		// 所持金から引く
		money.SubItemMoney(ID);
		// 購入済みリストに追加
		buyItemIDList.push_back(ID);
	}
	else if (!inputHandle && CheckHitKey(KEY_INPUT_3)) {
		inputHandle = true;
		int ID = 2;
		if (money.GetCurrentMoney() < itemDataList[ID]->price) return;

		// SEの再生
		PlaySE("DebugSE");
		// 所持金から引く
		money.SubItemMoney(ID);
		// 購入済みリストに追加
		buyItemIDList.push_back(ID);
	}
	else if (!inputHandle && CheckHitKey(KEY_INPUT_4)) {
		inputHandle = true;
		int ID = 3;
		if (money.GetCurrentMoney() < itemDataList[ID]->price) return;

		// SEの再生
		PlaySE("DebugSE");
		// 所持金から引く
		money.SubItemMoney(ID);
		// 購入済みリストに追加
		buyItemIDList.push_back(ID);
	}
	else if (!inputHandle && CheckHitKey(KEY_INPUT_RETURN)) {
		// SEの再生
		PlaySE("DebugSE");
		inputHandle = true;
		isComplete = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [&engine, this]() {
			ActiveShop(engine, buyItemIDList);
		});
	}
	// キー離しで再度入力受付
	if (CheckHitKey(KEY_INPUT_1) == 0 && CheckHitKey(KEY_INPUT_2) == 0 && CheckHitKey(KEY_INPUT_3) == 0 && CheckHitKey(KEY_INPUT_4) == 0 && CheckHitKey(KEY_INPUT_RETURN) == 0) {
		inputHandle = false;
	}

}
/*
 *	@brief	描画処理
 */
void SelectionShop::Render() {
	DrawFormatString(50, 50, GetColor(255, 255, 255), "=== Selection Shop Sample ===");
	DrawFormatString(50, 80, GetColor(0, 255, 0), "1: HP");
	DrawFormatString(50, 100, GetColor(0, 255, 0), "2: Stamina");
	DrawFormatString(50, 120, GetColor(0, 255, 0), "3: Strength");
	DrawFormatString(50, 140, GetColor(0, 255, 0), "4: ResistTime");
	DrawFormatString(50, 160, GetColor(0, 255, 0), "BuyItemList : ");
	for (int i = 0, max = buyItemIDList.size(); i < max; i++) {
		DrawFormatString(180 + i + 100, 160, GetColor(255, 255, 255), "%d", buyItemIDList[i]);
	}
}
/*
 *	@brief	片付け処理
 */
void SelectionShop::Teardown() {
	buyItemIDList.clear();
}
