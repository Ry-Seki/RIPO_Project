/*
 *	@file	MenuPlayerStatus.cpp
 *	@author	Seki
 */

#include "MenuPlayerStatus.h"
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
#include "../../../System/Status/PlayerStatusManager.h"
#include "../../../Manager/FontManager.h"

/*
 *	@brief	初期化処理
 */
void MenuPlayerStatus::Initialize(Engine& engine) {
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

            button->RegisterOnClick([this, &engine]() {
                SelectButtonExecute(engine);
            });
        }
        eventSystem.LoadNavigation(navigation->GetData());
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuPlayerStatus::Open() {
    MenuBase::Open();
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    currentStatus = PlayerStatusManager::GetInstance().GetPlayerStatusData();
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        eventSystem.ApplySelection();
        InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    });
}
/*
 *	@brief	更新処理
 */
void MenuPlayerStatus::Update(Engine& engine, float unscaledDeltaTime) {
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

    if (!inputHandle && input.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]) {
        inputHandle = true;
        button->OnPressDown();
    }
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuPlayerStatus::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
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
void MenuPlayerStatus::Render() {
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible()) continue;
        sprite->Render();
    }
    for (auto& button : buttonList) {
        if (!button->IsVisible()) continue;
        button->Render();
    }
    // 現在ステータスの描画
    CurrentStatusRender();
    // ステータスの比較
    if (isCallback) ComparisonStatus();
}
/*
 *	@brief	メニューを閉じる
 */
void MenuPlayerStatus::Close(Engine& engine) {
    MenuBase::Close(engine);
    isCallback = false;
}
/*
 *	@brief	ボタンの押された時の処理
 */
void MenuPlayerStatus::SelectButtonExecute(Engine& engine) {
    auto& menu = MenuManager::GetInstance();
    FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
        ExecuteCallback();
        menu.CloseTopMenu();
    });
}
/*
 *	@brief	現在のステータスの描画
 */
void MenuPlayerStatus::CurrentStatusRender() {
    auto& font = FontManager::GetInstance();
    constexpr int STATUS_COUNT = 4;

    for (int i = 0; i < STATUS_COUNT; ++i) {
        font.Draw("MiniSizeFont", 1425, 424 + 125 * i, std::to_string(currentStatus.base[i]).c_str(), GetColor(255, 255, 255));
    }
}
/*
 *	@brief	ステータスの比較処理
 */
void MenuPlayerStatus::ComparisonStatus() {
    auto& font = FontManager::GetInstance();

    const PlayerStatusValue& prev = prevStatus.base;
    const PlayerStatusValue& current = currentStatus.base;

    constexpr int STATUS_COUNT = 4;

    const float diffX = 1560.0f;   // +の位置
    const float baseY = 424;    // HP のY
    const float offsetY = 125;  // 各行間隔
    const int white = GetColor(255, 255, 255);

    for (int i = 0; i < STATUS_COUNT; ++i) {
        font.Draw("MiniSizeFont", 1170, 424 + 125 * i, std::to_string(prev[i]).c_str(), white);

        int diff = current[i] - prev[i];

        if (diff == 0) continue;

        // +表記
        std::string diffText = (diff > 0 ? "+" : "") + std::to_string(diff);

        float posY = baseY + offsetY * i;

        font.Draw(
            "MiniSizeFont",
            diffX,
            posY,
            diffText.c_str(),
            GetColor(255, 255, 255)
        );
    }
}

void MenuPlayerStatus::ExecuteCallback() {
    if (isCallback && Callback) {
        Callback();
    } else {
        FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 0.5f, FadeDirection::In, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeIn);
    }
}
