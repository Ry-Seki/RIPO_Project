/*
 *	@file	MenuMiniGame.cpp
 *	@author	Seki
 */

#include "MenuMiniGame.h"
#include "../../../Audio/AudioUtility.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../Load/Sprite/LoadSprite.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Input/InputUtility.h"
#include "../../../Save/SaveDataManager.h"
#include "../../../UI/Button/SinglePressButton.h"
#include "../../../Engine.h"
#include "../../../Scene/MainGameScene.h"
#include "../../../../Data/UI/MenuInfo.h"
#include "../../MenuResourcesFactory.h"
#include "../../MenuManager.h"
#include "../../System/MenuConfirm.h"
#include "../../../Scene/GameState/InAction/PartTime/MiniGame/MiniGameBase.h"
#include "../../../Scene/GameState/InAction/PartTime/MiniGame/Sokoban/MiniGameSokoban.h"

/*
 *	@brief	初期化処理
 */
void MenuMiniGame::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);

    load.SetOnComplete([this, &engine, menuJSON]() {
        MenuInfo result = MenuResourcesFactory::Create(menuJSON->GetData());
        for (auto& button : result.buttonList) {
            if (!button) continue;

            eventSystem.RegisterButton(button.get());
        }
        spriteList = std::move(result.spriteList);
        buttonList = std::move(result.buttonList);
        for (int i = 0, max = buttonList.size(); i < max; i++) {
            UIButtonBase* button = buttonList[i].get();
            if (!button) continue;

            button->RegisterOnClick([this, &engine, i]() {
                SelectButtonExecute(engine, i);
            });
        }
    });
    miniGameList.push_back(std::make_shared<MiniGameSokoban>());
}
/*
 *	@brief	メニューを開く
 */
void MenuMiniGame::Open() {
    MenuBase::Open();
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    miniGame = miniGameList[0];
    miniGame->SetMiniGameLevel(level);
    miniGame->Open();

    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        isStart = true;
        InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    });
}
/*
 *	@brief	更新処理
 */
void MenuMiniGame::Update(Engine& engine, float unscaledDeltaTime) {
    if (!isStart) return;

    auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);
    // ミニゲームの更新
    if(miniGame) miniGame->Update(unscaledDeltaTime);

    // ミニゲーム終了時、その結果を反映
    if (miniGame->IsComplete()) {
        isStart = false;
        AudioUtility::PlaySE("DebugSE");
        FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fade, [this]() {
            MenuManager::GetInstance().CloseTopMenu();
            if (Callback) Callback(level);
         });
    } else {
        // ボタンの更新
        for (auto& button : buttonList) {
            if (button) button->Update(unscaledDeltaTime);
        }
    }
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuMiniGame::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
    animTimer += unscaledDeltaTime;

    if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
    animTimer = 0;

    for (auto& sprite : spriteList) {
        int frameCount = sprite->GetFrameCount();
        if (frameCount <= 1) continue;

        animFrame = (animFrame + 1) % frameCount;
        sprite->SetFrameIndex(animFrame);
    }
}
/*
 *	@brief	描画処理
 */
void MenuMiniGame::Render() {
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible()) continue;
        sprite->Render();
    }
    for (auto& button : buttonList) {
        if (!button->IsVisible()) continue;
        button->Render();
    }
    if(miniGame) miniGame->Render();
}
/*
 *	@brief	メニューを閉じる
 */
void MenuMiniGame::Close(Engine& engine) {
    MenuBase::Close(engine);
    miniGame = nullptr;
}
/*
 *	@brief	メニューを再開
 */
void MenuMiniGame::Resume() {
    MenuBase::Resume();
    for (auto& button : buttonList) {
        button->Setup();
    }
    isStart = true;
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuMiniGame::SelectButtonExecute(Engine& engine, int buttonIndex) {
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();

    if(buttonIndex == 0){
        Reset();
    } else if (buttonIndex == 1) {
        confirm->SetCallback([this, &menu](GameEnum::ConfirmResult result) {
            if (result == GameEnum::ConfirmResult::Yes) {
                level = GameEnum::MiniGameLevel::Retire;
                menu.CloseTopMenu();
                isInteractive = false;
                FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
                FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
                    menu.CloseTopMenu();
                    if (Callback) Callback(level);
                });
            }else {
                menu.CloseTopMenu();
            }
        });
        menu.OpenMenu<MenuConfirm>();
    }
}
/*
 *	@brief		リセット処理
 */
void MenuMiniGame::Reset() {
    if(miniGame) miniGame->Reset();
    for (auto& button : buttonList) {
        button->Setup();
    }
}
