/*
 *	@file	ResultAction_Dungeon.cpp
 *	@author	Seki
 */

#include "ResultAction_Dungeon.h"
#include "../GameState_ResultAction.h"
#include "../../GameStateMachine.h"
#include "../../../../System/Money/MoneyManager.h"
#include "../../../../Audio/AudioUtility.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void ResultAction_Dungeon::Initialize() {
}
/*
 *	@brief	準備前処理
 */
void ResultAction_Dungeon::Setup() {
	prevMoney = 0;
	currentMoney = 0;
	inputHandle = false;
	isStart = false;
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		isStart = true;
		ResultPlayerAction();
	});
}
/*
 *	@brief	更新処理
 */
void ResultAction_Dungeon::Update(float deltaTime) {
	if (!isStart) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_SPACE)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		AdvanceDay();
	}
}
/*
 *	@brief	描画処理
 */
void ResultAction_Dungeon::Render() {
	if (!isStart) return;

	if (isPlayerDead) {
		DrawFormatString(50, 50, GetColor(255, 255, 255), "プレイヤーは死んでしまった...。");
	}else {
		DrawFormatString(50, 50, GetColor(255, 255, 255), "前 : %d", prevMoney);
		DrawFormatString(70, 70, GetColor(255, 255, 255), "↓  %d", currentMoney - prevMoney);
		DrawFormatString(50, 90, GetColor(255, 255, 255), "現在 : %d", currentMoney);
	}
	DrawFormatString(50, 180, GetColor(255, 255, 255), "次の日に進む->SpaceKey");
}
/*
 *	@brief	片付け処理
 */
void ResultAction_Dungeon::Teardown() {
	inputHandle = false;
}
/*
 *	@brief	日にち進行処理
 */
void ResultAction_Dungeon::AdvanceDay() {
	// 一日進める
	auto& context = owner->GetOwner()->GetActionContext();
	int elapsedDay = ++context.elapsedDay;
	// アクション終了フラグの変更
	owner->GetOwner()->SetIsActionEnd(true);
}
/*
 *	@brief	プレイヤーのリザルト処理
 */
void ResultAction_Dungeon::ResultPlayerAction() {
	auto& context = owner->GetOwner()->GetActionContext();

	context.currentIncome = 0;

	if (context.isPlayerDead) return;

	int prev = MoneyManager::GetInstance().GetPrevMoney();
	int current = MoneyManager::GetInstance().GetCurrentMoney();

	// すでに処理済みの収入イベントなら何もしない
	if (current == context.prevIncome) {
		prevMoney = current;
		currentMoney = current;
		return;
	}

	int delta = current - prev;
	if (delta > 0) {
		// 収入として確定
		context.currentIncome = delta;

		// イベント消費済みマーク
		context.prevIncome = current;

		// 表示用（そのまま）
		prevMoney = prev;
		currentMoney = current;
	}
}
