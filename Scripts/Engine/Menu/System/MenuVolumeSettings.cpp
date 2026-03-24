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

namespace {
    constexpr const float _MOVE_VALUE = 0.1f;
    constexpr const int _UI_MAX_VOLUME = 10;

    constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/System/Settings/SettingsResources.json";
    constexpr const char* _NAVIGATION_PATH = "Data/UI/System/Settings/SettingsNavigation.json";
    constexpr const char* _VOLUME_BUTTON_DATA_PATH = "Data/UI/System/Settings/SettingsButtonData.json";

    // 別名定義
    using VolumeType = GameEnum::VolumeType;
    using VolumeChangeType = GameEnum::VolumeChangeType;

    // 関数ポインタの別名定義
    using ExecuteChangeVolume = void(SettingsManager::*)(int);

    constexpr int _MAX_VOLUME_TYPE = static_cast<int>(GameEnum::VolumeType::Max);
    constexpr int _MAX_CHANGE_TYPE = static_cast<int>(GameEnum::VolumeChangeType::Max);

    // 音量調整処理の関数テーブル
    const ExecuteChangeVolume executeVolume[_MAX_VOLUME_TYPE][_MAX_CHANGE_TYPE] = {
        {
            &SettingsManager::SubMasterVolume,
            &SettingsManager::AddMasterVolume
        },
        {
            &SettingsManager::SubBGMVolume,
            &SettingsManager::AddBGMVolume
        },
        {
            &SettingsManager::SubSEVolume,
            & SettingsManager::AddSEVolume
        }
    };
    /*
     *  @brief  音量調整ボタンデータ構造体
     */
    struct VolumeButtonData {
        std::string name = "";
        VolumeType volumeType = VolumeType::Invalid;
        VolumeChangeType changeType = VolumeChangeType::Invalid;
    };
    /*
     *  @brief      音量の種類識別
     *  @param[in]  const std::string& typeKey
     *  @return     VolumeType
     */
    VolumeType StringToVolumeType(const std::string& typeKey) {
        if (typeKey == "Master") return VolumeType::Master;
        if (typeKey == "BGM")    return VolumeType::BGM;
        if (typeKey == "SE")     return VolumeType::SE;
        return VolumeType::Invalid;
    }
    /*
     *  @brief      音量調整の種類識別
     *  @param[in]  const std::string& typeKey
     *  @return     VolumeChangeType
     */
    VolumeChangeType StringToChangeType(const std::string& typeKey) {
        if (typeKey == "Down") return VolumeChangeType::Down;
        if (typeKey == "Up")   return VolumeChangeType::Up;
        return VolumeChangeType::Invalid;
    }
    /*
     *  @brief      JSON->音量ボタン情報へ変換
     *  @param[in]  const JSON& json
     *  @return     std::vector<VolumeButtonData>
     */
    std::vector<VolumeButtonData> ParseVolumeButtonData(const JSON& json) {
        std::vector<VolumeButtonData> result;

        auto array = json["VolumeButtons"];

        for (const auto& node : array) {
            VolumeButtonData data;

            data.name = node["ButtonName"].get<std::string>();
            data.volumeType = StringToVolumeType(node["VolumeType"].get<std::string>());
            data.changeType = StringToChangeType(node["ChangeType"].get<std::string>());

            result.push_back(data);
        }
        return result;
    }
}
/*
 *	@brief	初期化処理
 */
void MenuVolumeSettings::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
    auto buttonData = load.LoadResource<LoadJSON>(_VOLUME_BUTTON_DATA_PATH);

    load.SetOnComplete([this, &engine, menuJSON, navigation, buttonData]() {
        // メニューUI生成
        MenuInfo result = MenuResourcesFactory::Create(menuJSON->GetData());
        // メニューUIの所有権移動
        spriteList = std::move(result.spriteList);
        textList = std::move(result.textList);
        buttonList = std::move(result.buttonList);
        // ボタンの登録
        for (const auto& entry : buttonList) {
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
        InitializeVolumeButtons(buttonData->GetData());
        // イベントシステムのnavigationの設定
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
 *	@brief	メニューを再開
 */
void MenuVolumeSettings::Resume() {
    MenuBase::Resume();
    for (auto& button : buttonList) {
        button->Setup();
    }
}
/*
 *	@brief		ボタンが押されたときの処理
 *	@param[in]	GameEnum::VolumeType volumeType
 *	@param[in]	GameEnum::VolumeChangeType changeType
 */
void MenuVolumeSettings::SelectButtonExecute(GameEnum::VolumeType volumeType, GameEnum::VolumeChangeType changeType) {
    auto& settings = SettingsManager::GetInstance();
    AudioUtility::PlaySE("DebugSE");
    // 戻るボタン
    if (volumeType == GameEnum::VolumeType::Invalid
        || changeType == GameEnum::VolumeChangeType::Invalid) {
        OpenConfirmMenu();
        return;
    }
    int volumeTypeNum = static_cast<int>(volumeType);
    int changeTypeNum = static_cast<int>(changeType);
    // 音量調節の実行
    (settings.*executeVolume[volumeTypeNum][changeTypeNum])(1);
    // テキストの公人
    ChangeVolumeText(volumeTypeNum);
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
    if (textNum < 0 || textNum >= volumeTextList.size()) return;

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
/*
 *	@brief		音量ボタンの初期化処理
 *	@param[in]	const JSON& json
 */
void MenuVolumeSettings::InitializeVolumeButtons(const JSON& json) {
    auto volumeData = ParseVolumeButtonData(json);

    for (const auto& entry : volumeData) {
        UIButtonBase* button = FindButtonByName(entry.name);
        if (!button) continue;
        const auto volume = entry.volumeType;
        const auto change = entry.changeType;
        // ボタン実行処理の登録
        button->RegisterOnClick([this, volume, change]() {
            SelectButtonExecute(volume, change);
        });
        // ボタンに navigation 更新処理を登録
        button->RegisterUpdateSelectButton([this, button]() {
            eventSystem.UpdateSelectButton(button);
        });
    }
}
/*
 *	@brief		確認メニューを開く
 */
void MenuVolumeSettings::OpenConfirmMenu() {
    auto& menu = MenuManager::GetInstance();
    auto& settings = SettingsManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();
    confirm->SetCallback([this, &settings, &menu](GameEnum::ConfirmResult result) {
        menu.CloseTopMenu();        // 確認メニュー
        if (result != GameEnum::ConfirmResult::Yes) return;
        isInteractive = false;
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, &settings]() {
            settings.Save();
            menu.CloseTopMenu();    // このメニュー
        });
    });
    menu.OpenMenu<MenuConfirm>();
}
/*
 *	@brief		名前でのボタン検索
 *	@param[in]	const std::string& buttonName
 *	@return		UIButtonBase*
 */
UIButtonBase* MenuVolumeSettings::FindButtonByName(const std::string& buttonName) {
    auto itr = buttonMap.find(buttonName);
    if (itr == buttonMap.end()) return nullptr;

    return itr->second;
}
