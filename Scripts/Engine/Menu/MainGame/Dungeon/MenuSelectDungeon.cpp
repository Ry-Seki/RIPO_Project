/*
 *	@file	MenuSelectDungeon.cpp
 *	@author	Seki
 */

#include "MenuSelectDungeon.h"
#include "../../../Audio/AudioUtility.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../Load/Sprite/LoadSprite.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Input/InputUtility.h"
#include "../../../Save/SaveDataManager.h"
#include "../../../UI/Button/SinglePressButton.h"
#include "../../../GameConst.h"
#include "../../../Engine.h"
#include "../../../Scene/MainGameScene.h"
#include "../../../../Data/UI/MenuInfo.h"
#include "../../MenuResourcesFactory.h"
#include "../../MenuManager.h"
#include "../../System/MenuConfirm.h"
#include "../MenuInGame.h"

/*
 *	@brief	初期化処理
 */
void MenuSelectDungeon::Initialize(Engine& engine) {
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
void MenuSelectDungeon::Open() {
    MenuBase::Open();
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        // TODO : イベントごとに画像差し替え

        eventSystem.ApplySelection();
        InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    });
}
/*
 *	@brief	更新処理
 */
void MenuSelectDungeon::Update(Engine& engine, float unscaledDeltaTime) {
    auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

    if (input.buttonDown[static_cast<int>(GameEnum::MenuAction::Cancel)]) {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 0.5f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this]() {
            MenuManager::GetInstance().OpenMenu<MenuInGame>();
        });
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

    if (input.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]) {
        button->OnPressDown();
    }
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuSelectDungeon::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
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
void MenuSelectDungeon::Render() {
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
void MenuSelectDungeon::Close(Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief	メニューを中断
 */
void MenuSelectDungeon::Suspend() {
    MenuBase::Suspend();
}
/*
 *	@brief	メニューを再開
 */
void MenuSelectDungeon::Resume() {
    MenuBase::Resume();
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuSelectDungeon::SelectButtonExecute(Engine& engine, int buttonIndex) {
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();
    int dungeonID = 0;

    if (buttonIndex == 0) {
        dungeonID = 1;
        AudioUtility::PlaySE("DebugSE");
        confirm->SetCallback([this, &menu, dungeonID](GameEnum::ConfirmResult result) {
            if (result == GameEnum::ConfirmResult::Yes) {
                FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
                FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, dungeonID]() {
                    menu.CloseAllMenu();
                    if (Callback) Callback(dungeonID);
                });
            }
            menu.CloseTopMenu();
        });
        menu.OpenMenu<MenuConfirm>();
    } else if (buttonIndex == 1) {
        dungeonID = 2;
        AudioUtility::PlaySE("DebugSE");
        confirm->SetCallback([this, &menu, dungeonID](GameEnum::ConfirmResult result) {
            if (result == GameEnum::ConfirmResult::Yes) {
                FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
                FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, dungeonID]() {
                    menu.CloseAllMenu();
                    if (Callback) Callback(dungeonID);
                });
            }
            menu.CloseTopMenu();
        });
        menu.OpenMenu<MenuConfirm>();
    }else if (buttonIndex == 2) {
        dungeonID = 3;
        AudioUtility::PlaySE("DebugSE");
        confirm->SetCallback([this, &menu, dungeonID](GameEnum::ConfirmResult result) {
            if (result == GameEnum::ConfirmResult::Yes) {
                FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
                FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, dungeonID]() {
                    menu.CloseAllMenu();
                    if (Callback) Callback(dungeonID);
                });
            }
            menu.CloseTopMenu();
        });
        menu.OpenMenu<MenuConfirm>();
    }else if (buttonIndex == 3) {
        dungeonID = 4;
        AudioUtility::PlaySE("DebugSE");
        confirm->SetCallback([this, &menu, dungeonID](GameEnum::ConfirmResult result) {
            if (result == GameEnum::ConfirmResult::Yes) {
                FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
                FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, dungeonID]() {
                    menu.CloseAllMenu();
                    if (Callback) Callback(dungeonID);
                });
            }
            menu.CloseTopMenu();
        });
        menu.OpenMenu<MenuConfirm>();
    }

}
