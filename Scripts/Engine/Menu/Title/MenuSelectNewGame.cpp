/*
 *	@file	MenuSelectNewGame.cpp
 *	@author	Seki
 */

#include "MenuSelectNewGame.h"
#include "../MenuManager.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../../Audio/AudioUtility.h"
#include "../../Engine.h"
#include "../../Scene/MainGameScene.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Load/LoadManager.h"
#include "../../Input/InputUtility.h"

/*
 *	@brief	初期化処理
 */
void MenuSelectNewGame::Initialize(Engine& engine) {
}
/*
 *	@brief	メニューを開く
 */
void MenuSelectNewGame::Open() {
	MenuBase::Open();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
		InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
	});
}
/*
 *	@brief	更新処理
 */
void MenuSelectNewGame::Update(Engine& engine, float unscaledDeltaTime) {
	if (!isStart) return;
	auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);
	MenuManager& menu = MenuManager::GetInstance();

	if (input.buttonDown[static_cast<int>(GameEnum::MenuAction::Cancel)]) {
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
			menu.CloseTopMenu();
		});
	}
	// TODO : チュートリアルに行くようにする
	if (input.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]
		|| input.buttonDown[static_cast<int>(GameEnum::MenuAction::Click)]) {
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		engine.StartFadeOutIn(0.5f, 0.5f, [&engine, &menu]() {
			menu.CloseAllMenu();
			engine.SetNextScene(std::make_shared<MainGameScene>());
		});
	}
}
/*
 *	@brief	描画処理
 */
void MenuSelectNewGame::Render() {
	DrawFormatString(50, 70, GetColor(255, 255, 255), "NewGame");
	DrawFormatString(300, 400, GetColor(255, 255, 255), "Play->SpaceKey");
	DrawFormatString(300, 450, GetColor(255, 255, 255), "Return->EscapeKey");
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectNewGame::Close(Engine& engine) {
	MenuBase::Close(engine);
}
/*
 *	@brief	メニューを中断
 */
void MenuSelectNewGame::Suspend() {
	MenuBase::Suspend();
}
/*
 *	@brief	メニューを再開
 */
void MenuSelectNewGame::Resume() {
	MenuBase::Resume();
}
