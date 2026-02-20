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
#include "../MenuManager.h"
#include "../Title/MenuSelectNewGame.h"
#include "../Title/MenuSelectLoadGame.h"
#include "../Title/MenuSystem.h"
#include "../System/MenuConfirm.h"
#include "../MenuResourcesFactory.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Input/InputUtility.h"
#include "../../Scene/TutorialScene.h"

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
		for (auto& sprite : spriteList) {
			if (sprite->GetName() == "SelectTitleMenu_BackGround")
				backgroundSprite = sprite.get();
		}
		eventSystem.LoadNavigation(navigation->GetData());
	});

}
/*
 *	@brief	メニューを開く
 */
void MenuGameModeSelect::Open() {
	MenuBase::Open();
	for (auto& sprite : spriteList) {
		sprite->Setup();
	}
	for (auto& button : buttonList) {
		button->Setup();
	}
	eventSystem.ApplySelection();
	InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
}
/*
 *	@brief	更新処理
 */
void MenuGameModeSelect::Update(Engine& engine, float unscaledDeltaTime) {
	auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

	if (input.buttonDown[static_cast<int>(GameEnum::MenuAction::Cancel)]) {
		CheckEndGame(engine);
	}
	// イベントシステムの更新
	eventSystem.Update(unscaledDeltaTime);
	// ボタンの更新
	for (auto& button : buttonList) {
		if (button) button->Update(unscaledDeltaTime);
	}
	// 現在選択されているボタンの取得
	auto button = eventSystem.GetCurrentSelectButton();
	if (!button) return;

	if (!inputHandle && input.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]) {
		inputHandle = true;
		button->OnPressDown();
	}
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuGameModeSelect::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
	animTimer += unscaledDeltaTime;

	if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
	animTimer = 0;

	for (auto& sprite : spriteList) {
		if (!sprite) continue;

		int frameCount = sprite->GetFrameCount();
		if (frameCount <= 1) continue;

		animFrame = (animFrame + 1) % frameCount;
		sprite->SetFrameIndex(animFrame);
	}
}
/*
 *	@brief	描画処理
 */
void MenuGameModeSelect::Render() {
	if (backgroundSprite->IsVisible()) {
		backgroundSprite->Render();
	}
	for (auto& button : buttonList) {
		if (!button->IsVisible()) continue;
		button->Render();
	}
	for (auto& sprite : spriteList) {
		if (sprite.get() == backgroundSprite || !sprite->IsVisible()) continue;
		sprite->Render();
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
	for (auto& button : buttonList) {
		button->Setup();
	}
	eventSystem.ApplySelection();
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuGameModeSelect::SelectButtonExecute(Engine& engine, int buttonIndex) {
	auto& menu = MenuManager::GetInstance();
	auto confirm = menu.GetMenu<MenuConfirm>();
	if (buttonIndex == 0) {
		AudioUtility::PlaySE("DebugSE");
		confirm->SetCallback([this, confirm, &menu, &engine](GameEnum::ConfirmResult result) {
			if (result == GameEnum::ConfirmResult::Yes) {
				AudioUtility::PlaySE("DebugSE");
				menu.CloseAllMenu();
				engine.SetNextScene(std::make_shared<TutorialScene>());
			}
			menu.CloseMenu(confirm);
		});
		menu.OpenMenu<MenuConfirm>();
	} else if (buttonIndex == 1) {
		AudioUtility::PlaySE("DebugSE");
		FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 0.5f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
			menu.OpenMenu<MenuSelectLoadGame>();
		});
	} else if (buttonIndex == 2) {
		AudioUtility::PlaySE("DebugSE");
		FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 0.5f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
			menu.OpenMenu<MenuSystem>();
		});
	} else if (buttonIndex == 3) {
		CheckEndGame(engine);
	}
}
/*
 *	@brief		ゲーム終了処理
 */
void MenuGameModeSelect::CheckEndGame(Engine& engine) {
	auto& menu = MenuManager::GetInstance();
	auto confirm = menu.GetMenu<MenuConfirm>();
	AudioUtility::PlaySE("DebugSE");
	confirm->SetCallback([this, confirm, &menu, &engine](GameEnum::ConfirmResult result) {
		if (result == GameEnum::ConfirmResult::Yes) {
			AudioUtility::PlaySE("DebugSE");
			menu.CloseAllMenu();
			engine.SetIsGameEnd(true);
		}
		menu.CloseMenu(confirm);
						 });
	menu.OpenMenu<MenuConfirm>();
}
