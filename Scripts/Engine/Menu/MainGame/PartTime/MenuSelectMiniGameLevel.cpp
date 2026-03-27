/*
 *	@file	MenuSelectMiniGameLevel.cpp
 *	@author	Seki
 */

#include "MenuSelectMiniGameLevel.h"
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

namespace {
    /*
     *  @brief  ファイルパス
     */
    constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/PartTime/SelectLevel/SelectLevelMenuResources.json";
    constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/PartTime/SelectLevel/SelectLevelMenuNavigation.json";
    constexpr const char* _LEVEL_BUTTON_DATA_PATH = "Data/UI/MainGame/PartTime/SelectLevel/LevelButtonData.json";
    constexpr const char* _REWARD_TEXT_DATA_PATH = "Data/UI/MainGame/PartTime/SelectLevel/RewardTextData.json";

    // 別名定義
    using MiniGameLevel = GameEnum::MiniGameLevel;

    /*
     *  @brief  ミニゲーム難易度の種類マップ
     */
    const std::unordered_map<std::string, MiniGameLevel> levelTypeMap = {
        { "Easy", MiniGameLevel::Easy },
        { "Normal", MiniGameLevel::Normal },
        { "Hard", MiniGameLevel::Hard },
        { "Retire", MiniGameLevel::Retire },
        { "Back", MiniGameLevel::Invalid }
    };
    /*
     *  @brief  ミニゲーム難易度ボタン構造体
     */
    struct LevelButtonData {
        std::string name = "";
        MiniGameLevel level = MiniGameLevel::Invalid;
    };
    /*
     *  @brief  ミニゲーム難易度テキスト構造体
     */
    struct LevelRewardData {
        std::string name = "";
        MiniGameLevel level = MiniGameLevel::Invalid;
    };
    /*
     *  @brief      ミニゲーム難易度の種類識別
     *  @param[in]  const std::string& typeKey
     *  @return     MiniGameLevel
     */
    MiniGameLevel StringToMiniGameLevel(const std::string& typeKey) {
        auto itr = levelTypeMap.find(typeKey);

        if (itr != levelTypeMap.end()) return itr->second;

        return MiniGameLevel::Invalid;
    }
    /*
     *  @brief      JSON->難易度ボタン情報へ変換
     *  @param[in]  const JSON& json
     *	@return		std::vector<LevelButtonData>
     */
    std::vector<LevelButtonData> ParseLevelButtonData(const JSON& json) {
        std::vector<LevelButtonData> result;

        auto array = json["LevelButtons"];

        for (auto& node : array) {
            LevelButtonData entry;

            entry.name = node["ButtonName"].get<std::string>();

            std::string typeString = node["MiniGameLevel"].get<std::string>();
            entry.level = StringToMiniGameLevel(typeString);

            result.push_back(entry);
        }
        return result;
    }
    /*
     *  @brief      JSON->難易度報酬情報へ変換
     *  @param[in]  const JSON& json
     *	@return		std::vector<LevelRewardData>
     */
    std::vector<LevelRewardData> ParseLevelRewardData(const JSON& json) {
        std::vector<LevelRewardData> result;

        auto array = json["RewardTexts"];

        for (auto& node : array) {
            LevelRewardData entry;

            entry.name = node["TextName"].get<std::string>();

            std::string typeString = node["MiniGameLevel"].get<std::string>();
            entry.level = StringToMiniGameLevel(typeString);

            result.push_back(entry);
        }
        return result;
    }
}

/*
 *	@brief	初期化処理
 */
void MenuSelectMiniGameLevel::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
    auto buttonData = load.LoadResource<LoadJSON>(_LEVEL_BUTTON_DATA_PATH);
    auto textData = load.LoadResource<LoadJSON>(_REWARD_TEXT_DATA_PATH);

    load.SetOnComplete([this, &engine, menuJSON, navigation, buttonData, textData]() {
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
        // テキストの登録
        for (const auto& entry : textList) {
            if (!entry) continue;

            TextBase* text = entry.get();
            textMap[text->GetName()] = text;
        }
        // イベントシステムの初期化
        eventSystem.Initialize(0);
        // ボタンの初期化処理
        InitializeLevelButtons(buttonData->GetData());
        // 報酬テキストの初期化処化
        InitializeRewardTexts(textData->GetData());
        // イベントシステムのnavigationの設定
        eventSystem.LoadNavigation(navigation->GetData());
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuSelectMiniGameLevel::Open() {
    MenuBase::Open();
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    for (auto& text : textList) {
        text->Setup();
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
void MenuSelectMiniGameLevel::Update(Engine& engine, float unscaledDeltaTime) {
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
void MenuSelectMiniGameLevel::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
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
void MenuSelectMiniGameLevel::Render() {
    for (auto& sprite : spriteList) {
        if (!sprite || !sprite->IsVisible()) continue;
        sprite->Render();
    }
    for (auto& button : buttonList) {
        if (!button || !button->IsVisible()) continue;
        button->Render();
    }
    for (auto& text : textList) {
        if (!text) continue;
        text->Render();
    }
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectMiniGameLevel::Close(Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief	メニューを再開
 */
void MenuSelectMiniGameLevel::Resume() {
    MenuBase::Resume();
    for (auto& button : buttonList) {
        button->Setup();
    }
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	GameEnum::MiniGameLevel level
 */
void MenuSelectMiniGameLevel::SelectButtonExecute(GameEnum::MiniGameLevel level) {
    auto& menu = MenuManager::GetInstance();
    AudioUtility::PlaySE("DebugSE");
    // 戻る
    if (level == GameEnum::MiniGameLevel::Invalid) {
        StartFadeEndCallback(level);
        return;
    }
    // 確認メニューを開く
    OpenConfirmMenu(level);
}
/*
 *	@brief		フェード後->コールバックの実行処理
 *	@param[in]	GameEnum::MiniGameLevel level
 */
void MenuSelectMiniGameLevel::StartFadeEndCallback(GameEnum::MiniGameLevel level) {
    auto& menu = MenuManager::GetInstance();

    isInteractive = false;
    FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, level]() {
        menu.CloseTopMenu();	// このメニュー
        if (Callback) Callback(level);
    });
}
/*
 *	@brief		確認メニューを開く
 *	@param[in]	GameEnum::MiniGameLevel level
 */
void MenuSelectMiniGameLevel::OpenConfirmMenu(GameEnum::MiniGameLevel level) {
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();

    confirm->SetCallback([this, &menu, level](GameEnum::ConfirmResult result) {
        AudioUtility::PlaySE("DebugSE");
        menu.CloseTopMenu();    // 確認メニュー
        if (result != GameEnum::ConfirmResult::Yes) return;
        // フェード開始
        StartFadeEndCallback(level);
    });
    menu.OpenMenu<MenuConfirm>();
}
/*
 *	@brief		難易度ボタンの準備前処理
 *	@param[in]	const JSON& json
 */
void MenuSelectMiniGameLevel::InitializeLevelButtons(const JSON& json) {
    auto levelData = ParseLevelButtonData(json);

    for (const auto& entry : levelData) {
        UIButtonBase* button = FindButtonByName(entry.name);
        if (!button) continue;
        const auto level = entry.level;
        // ボタン実行処理の登録
        button->RegisterOnClick([this, level]() {
            SelectButtonExecute(level);
        });
        // ボタンに navigation 更新処理を登録
        button->RegisterUpdateSelectButton([this, button]() {
            eventSystem.UpdateSelectButton(button);
        });
    }
}
/*
 *	@brief		難易度別報酬テキストの準備前処理
 *	@param[in]	const JSON& json
 */
void MenuSelectMiniGameLevel::InitializeRewardTexts(const JSON& json) {
    auto levelData = ParseLevelRewardData(json);

    for (const auto& entry : levelData) {
        TextBase* text = FindTextByName(entry.name);
        if (!text) continue;
        const auto level = entry.level;
        if (level == GameEnum::MiniGameLevel::Invalid) continue;
        rewardMap[level] = text;
    }
}
/*
 *	@brief		名前でのボタン検索
 *	@param[in]	const std::string& buttonName
 *	@return		UIButtonBase*
 */
UIButtonBase* MenuSelectMiniGameLevel::FindButtonByName(const std::string& buttonName) {
    auto itr = buttonMap.find(buttonName);
    if (itr == buttonMap.end()) return nullptr;

    return itr->second;
}
/*
 *	@brief		名前でのテキスト検索
 *	@param[in]	const std::string& textName
 *	@return		TextBase*
 */
TextBase* MenuSelectMiniGameLevel::FindTextByName(const std::string& textName) {
    auto itr = textMap.find(textName);
    if (itr == textMap.end()) return nullptr;

    return itr->second;
}
/*
 *	@brief		難易度別報酬テキストの設定
 */
void MenuSelectMiniGameLevel::SetRewardTexts() {
    const int white = GetColor(75, 75, 75);
    const int commonReward = rewardList[static_cast<int>(GameEnum::MiniGameLevel::Retire)];

    for (const auto& [level, text] : rewardMap) {
        if (level == GameEnum::MiniGameLevel::Invalid || !text) continue;
        // 報酬の計算
        const int reward = rewardList[static_cast<int>(level)];
        const int clearBonus = reward - commonReward;
        std::string rewardText = std::to_string(clearBonus);
        // テキストに登録
        text->SetText(rewardText);
        text->SetColor(white);
    }
}