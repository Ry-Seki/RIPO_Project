/*
 *	@file	MenuSystem.cpp
 *	@author	Seki
 */

#include "MenuSystem.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../../Audio/AudioUtility.h"
#include "../../Engine.h"
#include "../../Scene/MainGameScene.h"
#include "../System/MenuSettings.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Input/InputUtility.h"
#include "../../Save/SaveDataManager.h"
#include "../MenuResourcesFactory.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Menu/MenuManager.h"
#include "../../GameConst.h"
#include "../../System/Settings/SettingsManager.h"

#include <DxLib.h>
/*
 *	@brief	初期化処理
 */
void MenuSystem::Initialize(Engine& engine) {
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
void MenuSystem::Open() {
	MenuBase::Open();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        currentIndex = -1;
        // ボタンの準備処理
        for (auto& button : buttonList) {
            button->Setup();
        }
        eventSystem.ApplySelection();
        InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    });
}
/*
 *	@brief	更新処理
 */
void MenuSystem::Update(Engine& engine, float unscaledDeltaTime) {
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
 *	@brief	描画処理
 */
void MenuSystem::Render() {
	DrawFormatString(50, 70, GetColor(255, 255, 255), "System");
    // ボタンの描画処理
    for (auto& button : buttonList) {
        button->Render();
    }
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSystem::Close(Engine& engine) {
	MenuBase::Close(engine);
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuSystem::SelectButtonExecute(Engine& engine, int buttonIndex) {
    auto& menu = MenuManager::GetInstance();
    if (buttonIndex == 0) {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
            isVisible = false;
            menu.OpenMenu<MenuSettings>();
        });

    } else if (buttonIndex == 1) {
        
    } else if (buttonIndex == 2) {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
            menu.CloseTopMenu();
        });
    }
}
