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
#include "../System/MenuVolumeSettings.h"
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
#include "../System/MenuCredit.h"

#include <DxLib.h>

namespace {
    /*
     *  @brief  ファイルパスの名前空間
     */
    constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/Title/SelectGameMode/System/SystemResources.json";
    constexpr const char* _NAVIGATION_PATH = "Data/UI/Title/SelectGameMode/System/SystemNavigation.json";
    constexpr const char* _SYSTEM_BUTTON_DATA_PATH = "Data/UI/Title/SelectGameMode/System/SystemButton.json";

    // 別名定義
    using SystemMode = GameEnum::SystemMode;

    /*
     *  @brief  システムの種類マップ
     */
    const std::unordered_map<std::string, SystemMode> systemTypeMap = {
        { "Settings", SystemMode::Settings },
        { "Credit", SystemMode::Credit },
        { "Back", SystemMode::Invalid }
    };
    /*
     *  @brief  システムボタン構造体
     */
    struct SystemButtonData {
        std::string name = "";
        SystemMode type = SystemMode::Invalid;
    };
    /*
     *	@brief	    システムの種類識別
     *  @param[in]  const std::string& typeKey
     *  @return     SystemMenuType
     */
    SystemMode StringToPlayerStatusType(const std::string& typeKey) {
        auto itr = systemTypeMap.find(typeKey);

        if (itr != systemTypeMap.end()) return itr->second;

        return GameEnum::SystemMode::Invalid;
    }
    /*
     *  @brief      JSON->トレーニングボタン情報へ変換
     *  @param[in]  const JSON& json
     */
    std::vector<SystemButtonData> ParseTrainingButtonData(const JSON& json) {
        std::vector<SystemButtonData> result;

        auto array = json["SystemButtons"];

        for (auto& node : array) {
            SystemButtonData entry;

            entry.name = node["ButtonName"].get<std::string>();

            std::string typeString = node["SystemType"].get<std::string>();
            entry.type = StringToPlayerStatusType(typeString);

            result.push_back(entry);
        }
        return result;
    }
}
/*
 *	@brief	初期化処理
 */
void MenuSystem::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
    auto buttonData = load.LoadResource<LoadJSON>(_SYSTEM_BUTTON_DATA_PATH);

    load.SetOnComplete([this, &engine, menuJSON, navigation, buttonData]() {
        // メニューのUI生成
        MenuInfo result = MenuResourcesFactory::Create(menuJSON->GetData());
        // メニューUIの所有権移動
        spriteList = std::move(result.spriteList);
        buttonList = std::move(result.buttonList);
        for (auto& entry : buttonList) {
            if (!entry) continue;

            UIButtonBase* button = entry.get();
            // ボタンの登録
            eventSystem.RegisterButton(button);
            // マップに登録
            buttonMap[button->GetName()] = button;
        }
        // イベントシステムの初期化
        eventSystem.Initialize(0);
        // ボタンの準備前処理
        SetupSystemButtons(buttonData->GetData());
        // ボタンの登録
        eventSystem.LoadNavigation(navigation->GetData());
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuSystem::Open() {
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

    if (!inputHandle && input.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]) {
        inputHandle = true;
        button->OnPressDown();
    }
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuSystem::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
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
void MenuSystem::Render() {
    // スプライトの描画
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible()) continue;

        sprite->Render();
    }
    // ボタンの描画処理
    for (auto& button : buttonList) {
        if (!button->IsVisible()) continue;

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
 *	@brief	メニューを再開
 */
void MenuSystem::Resume() {
    MenuBase::Resume();
    for (auto& button : buttonList) {
        button->Setup();
    }
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	GameEnum::SystemMenuType type
 */
void MenuSystem::SelectButtonExecute(GameEnum::SystemMode type) {
    auto& menu = MenuManager::GetInstance();
    FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);

    switch (type) {
        case GameEnum::SystemMode::Invalid:
            AudioUtility::PlaySE("DebugSE");
            menu.CloseTopMenu();
            break;
        case GameEnum::SystemMode::Settings:
            AudioUtility::PlaySE("DebugSE");
            isInteractive = false;
            FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
                menu.OpenMenu<MenuVolumeSettings>();
            });
            break;
        case GameEnum::SystemMode::Credit:
            AudioUtility::PlaySE("DebugSE");
            isInteractive = false;
            menu.OpenMenu<MenuCredit>();
            break;
        default:
            break;
    }
}
/*
 *	@brief		システムボタンの準備前処理
 *	@param[in]	const JSON& json
 */
void MenuSystem::SetupSystemButtons(const JSON& json) {
    auto systemData = ParseTrainingButtonData(json);

    for (const auto& entry : systemData) {
        UIButtonBase* button = FindButtonByName(entry.name);
        if (!button) continue;
        const auto type = entry.type;
        // ボタン実行処理の登録
        button->RegisterOnClick([this, type]() {
            SelectButtonExecute(type);
        });
        // ボタンに navigation 更新処理を登録
        button->RegisterUpdateSelectButton([this, button]() {
            eventSystem.UpdateSelectButton(button);
        });
    }
}
/*
 *	@brief		名前でのボタン検索
 *	@param[in]	const std::string& buttonName
 *	@return		UIButtonBase*
 */
UIButtonBase* MenuSystem::FindButtonByName(const std::string& buttonName) {
    auto itr = buttonMap.find(buttonName);
    if (itr == buttonMap.end()) return nullptr;

    return itr->second;
}