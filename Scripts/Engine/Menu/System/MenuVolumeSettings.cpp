/*
 *	@file	MenuVolumeSettings.cpp
 *	@author	Seki
 */

#include "MenuVolumeSettings.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Input/InputUtility.h"
#include "../../Save/SaveDataManager.h"
#include "../../Engine.h"
#include "../MenuResourcesFactory.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Menu/MenuManager.h"
#include "../../System/Settings/SettingsManager.h"
#include "MenuConfirm.h"
#include "../../Audio/AudioUtility.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void MenuVolumeSettings::Initialize(Engine& engine) {
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
        textList = std::move(result.textList);
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
void MenuVolumeSettings::Open() {
    MenuBase::Open();
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    SetVolumeText();
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        eventSystem.ApplySelection();
        InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    });
}
/*
 *	@brief	更新処理
 */
void MenuVolumeSettings::Update(Engine& engine, float unscaledDeltaTime) {
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
void MenuVolumeSettings::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
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
void MenuVolumeSettings::Render() {
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible()) continue;

        sprite->Render();
    }
    for (auto& text : textList) {
        text->Render();
    }
    for (auto& button : buttonList) {
        if (!button->IsVisible()) continue;

        button->Render();
    }
}
/*
 *	@brief	メニューを閉じる
 */
void MenuVolumeSettings::Close(Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuVolumeSettings::SelectButtonExecute(Engine& engine, int buttonIndex) {
    auto& settings = SettingsManager::GetInstance();
    auto& menu = MenuManager::GetInstance();
    int textNum = -1;

    if (buttonIndex == 0) {
        textNum = 0;
        AudioUtility::PlaySE("DebugSE");
        settings.SubMasterVolume(1);
        ChangeVolumeText(textNum);
    } else if (buttonIndex == 1) {
        textNum = 0;
        AudioUtility::PlaySE("DebugSE");
        settings.AddMasterVolume(1);
        ChangeVolumeText(textNum);
    } else if (buttonIndex == 2) {
        textNum = 1;
        AudioUtility::PlaySE("DebugSE");
        settings.SubBGMVolume(1);
        ChangeVolumeText(textNum);
    } else if (buttonIndex == 3) {
        textNum = 1;
        AudioUtility::PlaySE("DebugSE");
        settings.AddBGMVolume(1);
        ChangeVolumeText(textNum);
    } else if (buttonIndex == 4) {
        textNum = 2;
        AudioUtility::PlaySE("DebugSE");
        settings.SubSEVolume(1);
        ChangeVolumeText(textNum);
    } else if (buttonIndex == 5) {
        textNum = 2;
        AudioUtility::PlaySE("DebugSE");
        settings.AddSEVolume(1);
        ChangeVolumeText(textNum);
    } else if (buttonIndex == 6) {
        AudioUtility::PlaySE("DebugSE");
        auto confirm = menu.GetMenu<MenuConfirm>();
        confirm->SetCallback([this, &settings, &menu](GameEnum::ConfirmResult result) {
            if (result == GameEnum::ConfirmResult::Yes) {
                AudioUtility::PlaySE("DebugSE");
                menu.CloseTopMenu();
                isInteractive = false;
                FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
                FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, &settings]() {
                    settings.Save();
                    menu.CloseTopMenu();
                });
            }
            else {
                AudioUtility::PlaySE("DebugSE");
                menu.CloseTopMenu();
            }
        });
        menu.OpenMenu<MenuConfirm>();
    }
}
/*
 *	@brief		セーブデータから音量テキストの設定
 */
void MenuVolumeSettings::SetVolumeText() {
    auto& settings = SettingsManager::GetInstance();
    volumeTextList.clear();
    int textCount = textList.size();
    volumeTextList.resize(textCount);

    const int white = GetColor(255, 255, 255);
    // 音量の取得
    const int devideVolume[] = {
        settings.GetMasterVolume(),
        settings.GetBGMVolume(),
        settings.GetSEVolume()
    };
    // 音量の設定
    for (int i = 0; i < textCount; i++) {
        volumeTextList[i] = devideVolume[i];
        // テキストの設定
        textList[i]->SetText(std::to_string(volumeTextList[i]));
        textList[i]->SetColor(white);
    }
}
/*
 *	@brief		音量の変更処理
 *	@param[in]	int textNum
 */
void MenuVolumeSettings::ChangeVolumeText(int textNum) {
    auto& settings = SettingsManager::GetInstance();
    // 音量の取得
    const int devideVolume[] = {
        settings.GetMasterVolume(),
        settings.GetBGMVolume(),
        settings.GetSEVolume()
    };
    int volume = devideVolume[textNum];
    volumeTextList[textNum] = volume;
    textList[textNum]->SetText(std::to_string(volume));
}
