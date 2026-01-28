/*
 *	@file	MenuNewGame.cpp
 *	@author	Seki
 */

#include "MenuNewGame.h"
#include "../MenuManager.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../../Audio/AudioUtility.h"
#include "../../Engine.h"
#include "../../Scene/MainGameScene.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Load/LoadManager.h"

/*
 *	@brief	初期化処理
 */
void MenuNewGame::Initialize() {
}
/*
 *	@brief	メニューを開く
 */
void MenuNewGame::Open() {
	MenuBase::Open();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
	});
}
/*
 *	@brief	更新処理
 */
void MenuNewGame::Update(Engine& engine, float deltaTime) {
	if (!isStart) return;

	MenuManager& menu = MenuManager::GetInstance();
	if (!inputHandle) {
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			engine.StartFadeOutIn(0.5f, 0.5f, [&engine, &menu]() {
				menu.CloseAllMenu();
				engine.SetNextScene(std::make_shared<MainGameScene>());
			});
		} else if (CheckHitKey(KEY_INPUT_RETURN)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
			FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
				menu.CloseTopMenu();
			});
		}
	}
}
/*
 *	@brief	描画処理
 */
void MenuNewGame::Render() {
	DrawFormatString(50, 70, GetColor(255, 255, 255), "NewGame");
	DrawFormatString(300, 400, GetColor(255, 255, 255), "Play->SpaceKey");
	DrawFormatString(300, 450, GetColor(255, 255, 255), "Return->EnterKey");
}
/*
 *	@brief	メニューを閉じる
 */
void MenuNewGame::Close() {
	MenuBase::Close();
}
