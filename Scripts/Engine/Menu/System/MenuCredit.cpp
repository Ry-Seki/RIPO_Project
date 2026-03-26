/*
 *	@file	MenuCredit.cpp
 *	@author	Seki
 */

#include "MenuCredit.h"
#include "../MenuManager.h"
#include "../MenuResourcesFactory.h"
#include "../../Audio/AudioUtility.h"
#include "../../Fade/FadeManager.h"
#include "../../Fade/FadeFactory.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Input/InputUtility.h"
#include "../../../Data/UI/MenuInfo.h"

namespace {
	constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/Title/SelectGameMode/System/Credit/CreditMenuResources.json";
	constexpr const char* _NAVIGATION_PATH = "Data/UI/Title/SelectGameMode/System/Credit/CreditMenuNavigation.json";
}
/*
 *	@brief	初期化処理
 */
void MenuCredit::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);

    load.SetOnComplete([this, &engine, menuJSON, navigation]() {
        // メニューUI生成
        MenuInfo result = MenuResourcesFactory::Create(menuJSON->GetData());
        // メニューUIの所有権移動
        spriteList = std::move(result.spriteList);
        buttonList = std::move(result.buttonList);
        // ボタンの登録
        for (auto& button : buttonList) {
            if (!button) continue;
            // ボタンの登録
            eventSystem.RegisterButton(button.get());
            // ボタンの実行処理登録
            button->RegisterOnClick([this]() {
                SelectButtonExecute();
            });
            // ボタンに navigation 更新処理を登録
            button->RegisterUpdateSelectButton([this, button]() {
                eventSystem.UpdateSelectButton(button.get());
            });
        }
        // イベントシステムの初期化
        eventSystem.Initialize(0);
        // イベントシステムのnavigationの設定
        eventSystem.LoadNavigation(navigation->GetData());
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuCredit::Open() {
    MenuBase::Open();
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        eventSystem.ApplySelection();
        InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    });
}
/*
 *	@brief	更新処理
 */
void MenuCredit::Update(Engine& engine, float unscaledDeltaTime) {
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
void MenuCredit::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
    animTimer += unscaledDeltaTime;

    if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
    animTimer = 0;

    for (auto& sprite : spriteList) {
        if (!sprite) continue;
        int frameCount = sprite->GetFrameCount();
        if (frameCount <= 1) continue;

        int animFrame = sprite->GetCurrentFrame();
        animFrame = (animFrame + 1) % frameCount;
        sprite->SetFrameIndex(animFrame);
    }
}
/*
 *	@brief	描画処理
 */
void MenuCredit::Render() {
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
 *	@brief	ボタンの押された時の処理
 */
void MenuCredit::SelectButtonExecute() {
	AudioUtility::PlaySE("DebugSE");
	isInteractive = false;
    FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeOut, [this]() {
        MenuManager::GetInstance().CloseTopMenu();
    });
}
