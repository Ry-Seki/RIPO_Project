/*
 *	@file	MenuInGame.cpp
 *	@author	Seki
 */

#include "MenuInGame.h"
#include "../MenuManager.h"
#include "../System/MenuSettings.h"
#include "../System/MenuSaveMode.h"
#include "../System/MenuLoadMode.h"
#include "../../UI/Button/SinglePressButton.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Load/LoadManager.h"
#include "../../Input/InputUtility.h"
#include "../../Engine.h"
#include "../../Scene/TitleScene.h"
#include "../MenuResourcesFactory.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Menu/System/MenuConfirm.h"

/*
 *	@brief	初期化処理
 */
void MenuInGame::Initialize(Engine& engine) {
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
void MenuInGame::Open() {
    MenuBase::Open();
    currentSlot = -1;
    InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    for (auto& button : buttonList) {
        button->Setup();
    }
    eventSystem.ApplySelection();
}
/*
 *	@brief	更新処理
 */
void MenuInGame::Update(Engine& engine, float unscaledDeltaTime) {
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
void MenuInGame::Render() {
    DrawBox(0, 0, GameConst::WINDOW_WIDTH, GameConst::WINDOW_HEIGHT, GetColor(0, 0, 0), TRUE);
    for (auto& button : buttonList) {
        button->Render();
    }
    DrawFormatString(50, 250, GetColor(255, 255, 255), "InGameMenu");
}
/*
 *	@brief	メニューを閉じる
 */
void MenuInGame::Close(Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief	メニューを中断
 */
void MenuInGame::Suspend() {
    MenuBase::Suspend();
}
/*
 *	@brief	メニューを再開
 */
void MenuInGame::Resume() {
    MenuBase::Resume();
    eventSystem.ApplySelection();
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuInGame::SelectButtonExecute(Engine& engine, int buttonIndex) {
    auto& menu = MenuManager::GetInstance();
    if (buttonIndex == 0) {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
            menu.CloseTopMenu();
            menu.OpenMenu<MenuLoadMode>();
        });
    } else if (buttonIndex == 1) {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
            menu.CloseTopMenu();
            menu.OpenMenu<MenuSaveMode>();
        });
    } else if (buttonIndex == 2) {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
            menu.CloseTopMenu();
            menu.OpenMenu<MenuSettings>();
        });
    } else if (buttonIndex == 3) {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, &engine]() {
            menu.CloseTopMenu();
            // TODO : 確認メニューを出す
            auto confirm = menu.GetMenu<MenuConfirm>();
            confirm->SetCallback([&engine, &menu](GameEnum::ConfirmResult result) {
                if (result == GameEnum::ConfirmResult::Yes) {
                    menu.CloseAllMenu();
                    engine.SetNextScene(std::make_shared<TitleScene>());
                }else {
                    menu.CloseTopMenu();
                }
            });
            menu.OpenMenu<MenuConfirm>();
        });
    } else {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
            menu.CloseTopMenu();
        });
    }
}