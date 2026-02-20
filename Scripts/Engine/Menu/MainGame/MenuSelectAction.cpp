/*
 *	@brief	MenuSelectAction.cpp
 *	@author	Seki
 */

#include "MenuSelectAction.h"
#include "../../Audio/AudioUtility.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Fade/FadeManager.h"
#include "../../Fade/FadeFactory.h"
#include "../../Input/InputUtility.h"
#include "../../Save/SaveDataManager.h"
#include "../../UI/Button/SinglePressButton.h"
#include "../../GameConst.h"
#include "../../Engine.h"
#include "../../Scene/MainGameScene.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../MenuResourcesFactory.h"
#include "../MenuManager.h"
#include "../System/MenuConfirm.h"
#include "MenuInGame.h"
#include "../../Manager/FontManager.h"
#include "../../System/Status/PlayerStatusManager.h"
#include "Status/MenuPlayerStatus.h"
#include "../../System/Money/MoneyManager.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void MenuSelectAction::Initialize(Engine& engine) {
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
            if (sprite->GetName() == "ElapsedDay") elapsedDaySprite = sprite.get();
        }
        eventSystem.LoadNavigation(navigation->GetData());
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuSelectAction::Open() {
	MenuBase::Open();
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    if (isHalf) buttonList[0]->SetIsEnable(false);
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        isStart = true;
        if (isHalf) {
            elapsedDaySprite->SetFrameIndex(2);
        }
        else {
            elapsedDaySprite->SetFrameIndex(1);
        }

        eventSystem.ApplySelection();
        InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    });

}
/*
 *	@brief	更新処理
 */
void MenuSelectAction::Update(Engine& engine, float unscaledDeltaTime) {
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

    if (!inputHandle && input.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]) {
        inputHandle = true;
        button->OnPressDown();
    }
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuSelectAction::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
    animTimer += unscaledDeltaTime;

    if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
    animTimer = 0;

    for (auto& sprite : spriteList) {
        if (!sprite || sprite.get() == elapsedDaySprite) continue;

        int frameCount = sprite->GetFrameCount();
        if (frameCount <= 1) continue;

        animFrame = (animFrame + 1) % frameCount;
        sprite->SetFrameIndex(animFrame);
    }
}
/*
 *	@brief	描画処理
 */
void MenuSelectAction::Render() {
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible() || sprite.get() == elapsedDaySprite) continue;
        sprite->Render();
    }
    for (auto& button : buttonList) {
        if (!button->IsVisible()) continue;
        button->Render();
    }
    if(elapsedDaySprite) elapsedDaySprite->Render();

    std::string elapsedDayStr = std::to_string(elapsedDay);
    std::string maxDayStr = " / " + std::to_string(GameConst::END_DAY);
    std::string money = std::to_string(MoneyManager::GetInstance().GetCurrentMoney());
    FontManager::GetInstance().Draw("NormalSizeFont", 190, 510, elapsedDayStr, GetColor(75, 75, 75));
    FontManager::GetInstance().Draw("NormalSizeFont", 250, 510, maxDayStr, GetColor(75, 75, 75));
    FontManager::GetInstance().Draw("NormalSizeFont", 455, 973, money, GetColor(75, 75, 75));
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectAction::Close(Engine& engine) {
	MenuBase::Close(engine);
}
/*
 *	@brief	メニューを中断
 */
void MenuSelectAction::Suspend() {
	MenuBase::Suspend();
}
/*
 *	@brief	メニューを再開
 */
void MenuSelectAction::Resume() {
	MenuBase::Resume();
    for (auto& button : buttonList) {
        button->Setup();
    }
    if (isHalf) {
        buttonList[0]->SetIsEnable(false);
    }
    else {
        elapsedDaySprite->SetFrameIndex(1);
    }
    
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuSelectAction::SelectButtonExecute(Engine& engine, int buttonIndex) {
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();
    GameEnum::ActionType type = GameEnum::ActionType::Invalid;

    if (buttonIndex == 0) {
        AudioUtility::PlaySE("DebugSE");
        type = GameEnum::ActionType::Dungeon;
        confirm->SetCallback([this, &menu, &engine, type](GameEnum::ConfirmResult result){
            if (result == GameEnum::ConfirmResult::Yes) {
                FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
                FadeManager::GetInstance().StartFade(fade, [this, &menu, type]() {
                    menu.CloseTopMenu();
                    if (Callback) Callback(type);
                });
            }
            menu.CloseTopMenu();
        });
        menu.OpenMenu<MenuConfirm>();
    }else if(buttonIndex == 1){
        AudioUtility::PlaySE("DebugSE");
        type = GameEnum::ActionType::Training;
        confirm->SetCallback([this, &menu, &engine, type](GameEnum::ConfirmResult result) {
            if (result == GameEnum::ConfirmResult::Yes) {
                FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
                FadeManager::GetInstance().StartFade(fade, [this, &menu, type]() {
                    menu.CloseTopMenu();
                    if (Callback) Callback(type);
                });
            }
            menu.CloseTopMenu();
        });
        menu.OpenMenu<MenuConfirm>();
    } else if (buttonIndex == 2) {
        AudioUtility::PlaySE("DebugSE");
        type = GameEnum::ActionType::Shop;
        confirm->SetCallback([this, &menu, &engine, type](GameEnum::ConfirmResult result) {
            if (result == GameEnum::ConfirmResult::Yes) {
                FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
                FadeManager::GetInstance().StartFade(fade, [this, &menu, type]() {
                    menu.CloseTopMenu();
                    if (Callback) Callback(type);
                });
            }
            menu.CloseTopMenu();
        });
        menu.OpenMenu<MenuConfirm>();
    } else if (buttonIndex == 3) {
        AudioUtility::PlaySE("DebugSE");
        type = GameEnum::ActionType::PartTime;
        confirm->SetCallback([this, &menu, &engine, type](GameEnum::ConfirmResult result) {
            if (result == GameEnum::ConfirmResult::Yes) {
                FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
                FadeManager::GetInstance().StartFade(fade, [this, &menu, type]() {
                    menu.CloseTopMenu();
                    if (Callback) Callback(type);
                });
            }
            menu.CloseTopMenu();
        });
        menu.OpenMenu<MenuConfirm>();
    }else if (buttonIndex == 4) {
        AudioUtility::PlaySE("DebugSE");
        auto& menu = MenuManager::GetInstance();
        auto status = menu.GetMenu<MenuPlayerStatus>();
        status->SetIsCallback(false);
        status->SetPrevStatusData(PlayerStatusManager::GetInstance().GetPlayerStatusData());
        menu.OpenMenu<MenuPlayerStatus>();
    }
}
