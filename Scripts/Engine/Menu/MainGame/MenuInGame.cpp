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
        spriteList = std::move(result.spriteList);
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
    animTimer = 0.0f;
    animFrame = 0;
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
 *	@brief	アニメーション等の更新
 */
void MenuInGame::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
    animTimer += unscaledDeltaTime;

    if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
    animTimer -= GameConst::UI_ANIM_INTERVAL;

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
void MenuInGame::Render() {
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible()) continue;

        sprite->Render();
    }
    for (auto& button : buttonList) {
        if (!button->IsVisible()) continue;

        button->Render();
    }
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
            menu.OpenMenu<MenuLoadMode>();
        });
    } else if (buttonIndex == 1) {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
            menu.OpenMenu<MenuSaveMode>();
        });
    } else if (buttonIndex == 2) {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
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
            FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 0.5f, FadeDirection::In, FadeMode::Stop);
            FadeManager::GetInstance().StartFade(fadeIn, [this, &menu]() {
                menu.CloseTopMenu();
            });
        });
    }
}