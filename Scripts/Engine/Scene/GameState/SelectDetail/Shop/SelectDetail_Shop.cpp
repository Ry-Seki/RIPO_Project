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
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade);
}
/*
 *	@brief	更新処理
 */
void SelectDetail_Shop::Update(float deltaTime) {
	MoneyManager& money = MoneyManager::GetInstance();
	if (!inputHandle) {

	}
	if (CheckHitKey(KEY_INPUT_1)) {
		inputHandle = true;
		int ID = 0;

	}
	else if (CheckHitKey(KEY_INPUT_2)) {
		inputHandle = true;
		int ID = 1;

	}
	else if (CheckHitKey(KEY_INPUT_3)) {
		inputHandle = true;
		int ID = 2;

	}
	else if (CheckHitKey(KEY_INPUT_4)) {
		inputHandle = true;
		int ID = 3;

	}
	else if (CheckHitKey(KEY_INPUT_SPACE)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;

	}
	// キー離しで再度入力受付
	if (CheckHitKey(KEY_INPUT_1) == 0 && CheckHitKey(KEY_INPUT_2) == 0 
		&& CheckHitKey(KEY_INPUT_3) == 0 && CheckHitKey(KEY_INPUT_4) == 0 && CheckHitKey(KEY_INPUT_SPACE) == 0) {
		inputHandle = false;
	}

}
/*
 *	@brief	描画処理
 */
void SelectDetail_Shop::Render() {
}
/*
 *	@brief	片付け処理
 */
void SelectDetail_Shop::Teardown() {
}
