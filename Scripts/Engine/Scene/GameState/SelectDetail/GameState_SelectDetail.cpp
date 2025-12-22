/*
 *	@file	GameState_SelectDetail.cpp
 *	@author	Seki
 */

#include "GameState_SelectDetail.h"
#include "Dungeon/SelectDetail_Dungeon.h"
#include "Training/SelectDetail_Training.h"
#include "Shop/SelectDetail_Shop.h"
#include "PartTime/SelectDetail_PartTime.h"

#include "../../../GameEnum.h"

/*
 *	@brief	初期化処理
 */
void GameState_SelectDetail::Initialize() {
	// 行動の要素分をあらかじめ
	int maxCount = static_cast<int>(GameEnum::ActionType::Max);
	selectDetailList.resize(maxCount);
	// 各詳細クラスの生成
	selectDetailList[static_cast<int>(GameEnum::ActionType::Dungeon)]
		= std::make_shared<SelectDetail_Dungeon>();
	selectDetailList[static_cast<int>(GameEnum::ActionType::Training)]
		= std::make_shared<SelectDetail_Training>();
	selectDetailList[static_cast<int>(GameEnum::ActionType::Shop)]
		= std::make_shared<SelectDetail_Shop>();
	selectDetailList[static_cast<int>(GameEnum::ActionType::PartTime)]
		= std::make_shared<SelectDetail_PartTime>();
	// 各詳細クラスの初期化
	for (int i = 0; i < maxCount; i++) {
		auto detail = selectDetailList[i];
		if (!detail) continue;

		detail->Initialize();
	}
}
/*
 *	@brief	準備前処理
 */
void GameState_SelectDetail::Setup() {
	// currentDetail = selectdetailList[];
	// currentDetail->Setup();

}
/*
 *	@brief	更新処理
 */
void GameState_SelectDetail::Update(float deltaTime) {
	if (currentDetail) currentDetail->Update(deltaTime);

}
/*
 *	@brief	描画処理(デバッグ用)
 */
void GameState_SelectDetail::Render() {
	if (currentDetail) currentDetail->Render();
}
/*
 *	@brief	片付け処理
 */
void GameState_SelectDetail::Teardown() {
	if (currentDetail) currentDetail->Teardown();
}
/*
 *	@brief	ロード済みデータのセット(コールバック)
 */
void GameState_SelectDetail::SetupData() {
}
