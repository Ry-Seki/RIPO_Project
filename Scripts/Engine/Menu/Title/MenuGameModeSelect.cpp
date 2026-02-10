/*
 *	@file	MenuGameModeSelect.cpp
 *	@author	Seki
 */

#include "MenuGameModeSelect.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../../Audio/AudioUtility.h"
#include "../../../Engine/Engine.h"
#include "../../Scene/MainGameScene.h"
#include "../../Menu/MenuManager.h"
#include "../../Menu/Title/MenuSelectNewGame.h"
#include "../../Menu/Title/MenuSelectLoadGame.h"
#include "../../Menu/Title/MenuSystem.h"
#include "../MenuResourcesFactory.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Input/InputUtility.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void MenuGameModeSelect::Initialize(Engine& engine) {
	auto& load = LoadManager::GetInstance();
	auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
	auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);

	load.SetOnComplete([this, &engine, menuJSON, navigation]() {
		MenuInfo result = MenuResourcesFactory::Create(menuJSON->GetData());
		for (auto& button : result.buttonList) {
			if (!button) continue;

			eventSystem.RegisterButton(button.get());
		}
		eventSystem.Initialize(0);
		buttonList = std::move(result.buttonList);
		spriteList = std::move(result.spriteList);
		for (int i = 0, max = buttonList.size(); i < max; i++) {
			UIButtonBase* button = buttonList[i].get();
			if (!button) continue;

			button->RegisterUpdateSelectButton([this, button]() {
				eventSystem.UpdateSelectButton(button);
			});

			button->RegisterOnClick([this, &engine, i]() {
				SelectButtonExecute(engine, i);
			});
		}
		eventSystem.LoadNavigation(navigation->GetData());
	});

}
/*
 *	@brief	メニューを開く
 */
void MenuGameModeSelect::Open() {
	MenuBase::Open();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
	});
}
/*
 *	@brief	更新処理
 */
void MenuGameModeSelect::Update(Engine& engine, float unscaledDeltaTime) {
	if (!isStart) return;

	auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);
	// イベントシステムの更新
	eventSystem.Update(unscaledDeltaTime);
	// ボタンの更新
	for (auto& button : buttonList) {
		if (button) button->Update(unscaledDeltaTime);
	}
	// 現在選択されているボタンの取得
	auto button = eventSystem.GetCurrentSelectButton();
	if (!button) return;

	if (input.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]) {
		button->OnPressDown();
	}
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuGameModeSelect::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
}
/*
 *	@brief	描画処理
 */
void MenuGameModeSelect::Render() {
	for (auto& sprite : spriteList) {
		sprite->Render();
	}
	for (auto& button : buttonList) {
		button->Render();
	}
}
/*
 *	@brief	メニューを閉じる
 */
void MenuGameModeSelect::Close(Engine& engine) {
	MenuBase::Close(engine);
}
/*
 *	@brief	メニューを再開
 */
void MenuGameModeSelect::Resume() {
	MenuBase::Resume();
	isStart = false;
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
		inputHandle = false;
	});
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuGameModeSelect::SelectButtonExecute(Engine& engine, int buttonIndex) {
}
