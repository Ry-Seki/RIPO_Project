/*
 *	@file	ResultScene.cpp
 *	@author	Seki
 */

#include "ResultScene.h"
#include "TitleScene.h"
#include "../Engine.h"
#include "../Fade/FadeFactory.h"
#include "../Fade/FadeManager.h"
#include "../System/Money/MoneyManager.h"

#include <DxLib.h>

 /*
  *	コンストラクタ
  */
ResultScene::ResultScene() {
}
/*
 *	デストラクタ
 */
ResultScene::~ResultScene() {
}
/*
 *	初期化処理
 */
void ResultScene::Initialize(Engine& engine) {
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		isStart = true;
	});
}
/*
 *	ロード済みデータのセット (コールバック)
 */
void ResultScene::SetupData(Engine& engine) {
}
/*
 *	@brief	準備前処理
 */
void ResultScene::Setup(Engine& engine) {
}
/*
 *	更新処理
 */
void ResultScene::Update(Engine& engine, float deltaTime) {
	if (!isStart) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_SPACE)) {
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [&engine, this]() {
			engine.SetNextScene(std::make_shared<TitleScene>());
		});
	}
}
/*
 *	描画処理
 */
void ResultScene::Render() {
	MoneyManager& money = MoneyManager::GetInstance();
	DrawFormatString(50, 50, GetColor(255, 255, 255), "ResultMoney : %d", money.GetCurrentMoney());
	DrawFormatString(50, 70, GetColor(255, 255, 255), "ResultScore ");
}
