/*
 *	@file	MenuSelectTraining.cpp
 *	@author	Seki
 */

#include "MenuSelectTraining.h"
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

namespace {
    /*
     *  @brief  ファイルパスの名前空間
     */
    constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Training/SelectTraining/SelectTrainingMenuResources.json";
    constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/Training/SelectTraining/SelectTrainingMenuNavigation.json";
    constexpr const char* _TRAING_BUTTON_DATA_PATH = "Data/UI/MainGame/Training/SelectTraining/TrainingButtonData.json";

    // 別名定義
    using PlayerStatusType = GameEnum::PlayerStatusType;

    /*
     *  @brief  ステータスの種類マップ
     */
    const std::unordered_map<std::string, PlayerStatusType> statusTypeMap = {
        {"HP", ::PlayerStatusType::HP},
        {"Stamina", ::PlayerStatusType::Stamina},
        {"Strength", ::PlayerStatusType::Strength},
        {"ResistTime", ::PlayerStatusType::ResistTime},
        {"Back", ::PlayerStatusType::Invalid}
    };
    /*
     *  @brief  トレーニングボタン構造体
     */
    struct TrainingButtonData {
        std::string name = "";
        PlayerStatusType type = PlayerStatusType::Invalid;
    };
    /*
     *	@brief	    プレイヤーのステータスの種類識別
     *  @param[in]  const std::string& typeKey
     *  @return     PlayerStatusType
     */
    PlayerStatusType StringToPlayerStatusType(const std::string& typeKey) {
        auto itr = statusTypeMap.find(typeKey);

        if (itr != statusTypeMap.end()) return itr->second;

        return PlayerStatusType::Invalid;
    }
    /*
     *  @brief      JSON->トレーニングボタン情報へ変換
     *  @param[in]  const JSON& json
     */
    std::vector<TrainingButtonData> ParseTrainingButtonData(const JSON& json) {
        std::vector<TrainingButtonData> result;

        auto array = json["TrainingButtons"];

        for (const auto& node : array) {
            TrainingButtonData entry;

            entry.name = node["ButtonName"].get<std::string>();

            std::string typeString = node["StatusType"].get<std::string>();
            entry.type = StringToPlayerStatusType(typeString);

            result.push_back(entry);
        }
        return result;
    }
}
/*
 *	@brief	初期化処理
 */
void MenuSelectTraining::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();

    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
    auto buttonData = load.LoadResource<LoadJSON>(_TRAING_BUTTON_DATA_PATH);

    load.SetOnComplete([this, &engine, menuJSON, navigation, buttonData]() {
        // メニューUI生成
        MenuInfo result = MenuResourcesFactory::Create(menuJSON->GetData());
        // メニューUIの所有権移動
        spriteList = std::move(result.spriteList);
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
        SetupTrainingButtons(buttonData->GetData());
        // イベントシステムのnavigationの設定
        eventSystem.LoadNavigation(navigation->GetData());
     });
}
/*
 *	@brief	メニューを開く
 */
void MenuSelectTraining::Open() {
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
 *	@brief	メニューを閉じる
 */
void MenuSelectTraining::Update(Engine& engine, float unscaledDeltaTime) {
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
void MenuSelectTraining::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
    animTimer += unscaledDeltaTime;

    if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
    animTimer = 0;

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
void MenuSelectTraining::Render() {
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible()) continue;

        sprite->Render();
    }
    for (int i = buttonList.size() - 1; i >= 0; i--) {
        auto& button = buttonList[i];
        if (!button || !button->IsVisible()) continue;

        button->Render();
    }
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectTraining::Close(Engine& engine) {
    MenuBase::Close(engine);
    AudioUtility::StopBGM();
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	GameEnum::PlayerStatusType type
 */
void MenuSelectTraining::SelectButtonExecute(GameEnum::PlayerStatusType type) {
    AudioUtility::PlaySE("DebugSE");
    // 戻る
    if (type == GameEnum::PlayerStatusType::Invalid) {
        StartFadeEndCallback(type);
        return;
    }
    // 確認メニューを開く
    OpenConfirmMenu(type);
}
/*
 *	@brief		フェード後->コールバックの実行処理
 *	@param[in]	GameEnum::PlayerStatusType type
 */
void MenuSelectTraining::StartFadeEndCallback(GameEnum::PlayerStatusType type) {
    auto& menu = MenuManager::GetInstance();

    isInteractive = false;
    FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, type]() {
        menu.CloseTopMenu();	// このメニュー
        if (Callback) Callback(type);
    });
}
/*
 *	@brief		確認メニューを開く
 *	@param[in]	GameEnum::PlayerStatusType type
 */
void MenuSelectTraining::OpenConfirmMenu(GameEnum::PlayerStatusType type) {
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();

    confirm->SetCallback([this, &menu, type](GameEnum::ConfirmResult result) {
        AudioUtility::PlaySE("DebugSE");
        menu.CloseTopMenu();    // 確認メニュー
        if (result != GameEnum::ConfirmResult::Yes) return;
        // フェード開始
        StartFadeEndCallback(type);
    });
    menu.OpenMenu<MenuConfirm>();
}
/*
 *	@brief		トレーニングボタンの準備前処理
 *	@param[in]	const JSON& json
 */
void MenuSelectTraining::SetupTrainingButtons(const JSON& json) {
    auto trainingData = ParseTrainingButtonData(json);

    for (const auto& entry : trainingData) {
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
UIButtonBase* MenuSelectTraining::FindButtonByName(const std::string& buttonName) {
    auto itr = buttonMap.find(buttonName);
    if (itr == buttonMap.end()) return nullptr;

    return itr->second;
}
