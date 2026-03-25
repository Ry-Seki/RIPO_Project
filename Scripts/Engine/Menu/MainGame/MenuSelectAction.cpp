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
#include "../../System/Money/MoneyManager.h"
#include "../../Load/Audio/LoadAudio.h"

#include <DxLib.h>

namespace {
    /*
     *  @brief  ファイルパス
     */
    constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/SelectAction/SelectActionMenuResources.json";
    constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/SelectAction/SelectActionMenuNavigation.json";
    constexpr const char* _ACTION_BUTTON_DATA_PATH = "Data/UI/MainGame/SelectAction/ActionButtonData.json";
    constexpr const char* _SELECTMENU_BGMPATH = "Res/Audio/BGM/Title/MenuBGM.mp3";

    // 別名定義
    using ActionType = GameEnum::ActionType;

    /*
     *  @brief  アクションの種類マップ
     */
    const std::unordered_map<std::string, ActionType> actionTypeMap = {
        { "Dungeon", ActionType::Dungeon },
        { "Training", ActionType::Training },
        { "Shop", ActionType::Shop },
        { "PartTime", ActionType::PartTime },
        { "Status", ActionType::Max },
        { "InGameMenu", ActionType::Invalid }
    };
    /*
     *  @brief  アクションボタン構造体
     */
    struct ActionButtonData {
        std::string name = "";
        ActionType type = ActionType::Invalid;
    };
    /*
     *	@brief	    アクションの種類識別
     *  @param[in]  const std::string& typeKey
     *  @return     ActionType
     */
    ActionType StringToActionType(const std::string& typeKey) {
        auto itr = actionTypeMap.find(typeKey);

        if (itr != actionTypeMap.end()) return itr->second;

        return ActionType::Invalid;
    }
    /*
     *  @brief      JSON->アクションボタン情報へ変換
     *  @param[in]  const JSON& json
     *  @return     std::vector<ActionButtonData>
     */
    std::vector<ActionButtonData> ParseActionButtonData(const JSON& json) {
        std::vector<ActionButtonData> result;

        auto array = json["ActionButtons"];

        for (const auto& node : array) {
            ActionButtonData entry;

            entry.name = node["ButtonName"].get<std::string>();

            std::string typeString = node["ActionType"].get<std::string>();
            entry.type = StringToActionType(typeString);

            result.push_back(entry);
        }
        return result;
    }
}
/*
 *	@brief	初期化処理
 */
void MenuSelectAction::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
    auto buttonData = load.LoadResource<LoadJSON>(_ACTION_BUTTON_DATA_PATH);
    auto selectMenuBGM = load.LoadResource<LoadAudio>(_SELECTMENU_BGMPATH);

    load.SetOnComplete([this, &engine, menuJSON, navigation, buttonData, selectMenuBGM]() {
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
        for (auto& sprite : spriteList) {
            if (sprite->GetName() == "ElapsedDay") elapsedDaySprite = sprite.get();
        }
        // イベントシステムの初期化
        eventSystem.Initialize(0);
        // ボタンの準備前処理
        InitializeActionButtons(buttonData->GetData());
        // イベントシステムのnavigationの設定
        eventSystem.LoadNavigation(navigation->GetData());
        AudioUtility::RegisterBGMHandle(GameConst::_MENU_BGM, selectMenuBGM->GetHandle());
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuSelectAction::Open() {
	MenuBase::Open();
    AudioUtility::ChangeBGM(GameConst::_MENU_BGM);
    AudioUtility::PlayBGM();
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    if (isHalf) buttonList[0]->SetIsEnable(false);
    CreateElapsedDayText();
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        isStart = true;
        if (isHalf) {
            elapsedDaySprite->SetFrameIndex(1);
        }
        else {
            elapsedDaySprite->SetFrameIndex(0);
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
        isInteractive = false;
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
void MenuSelectAction::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
    animTimer += unscaledDeltaTime;

    if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
    animTimer = 0;

    for (auto& sprite : spriteList) {
        if (!sprite || sprite.get() == elapsedDaySprite) continue;

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
    for (auto& text : textList) {
        text->Render();
    }
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectAction::Close(Engine& engine) {
	MenuBase::Close(engine);
    AudioUtility::StopBGM();
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
}
/*
 *	@brief		テキストの生成
 */
void MenuSelectAction::CreateElapsedDayText() {
    const int gray = GetColor(75, 75, 75);
    std::string elapsedDayStr = std::to_string(elapsedDay);
    std::string money = std::to_string(MoneyManager::GetInstance().GetCurrentMoney());
    textList[0]->SetText(elapsedDayStr);
    textList[1]->SetText(money);
    for (auto& text : textList) {
        text->SetColor(gray);
    }
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	GameEnum::ActionType type
 */
void MenuSelectAction::SelectButtonExecute(GameEnum::ActionType type) {
    AudioUtility::PlaySE("DebugSE");
    StartFadeEndCallback(type);
}
/*
 *	@brief		アクションボタンの初期化処理
 *	@param[in]	const JSON& json
 */
void MenuSelectAction::InitializeActionButtons(const JSON& json) {
    auto actionData = ParseActionButtonData(json);

    for (const auto& entry : actionData) {
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
 *	@brief		フェード後->コールバックの実行処理
 *	@param[in]	GameEnum::ActionType type
 */
void MenuSelectAction::StartFadeEndCallback(GameEnum::ActionType type) {
    auto& menu = MenuManager::GetInstance();

    isInteractive = false;
    FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, type]() {
        if (Callback) Callback(type);
    });
}
/*
 *	@brief		名前でのボタン検索
 *	@param[in]	const std::string& buttonName
 *	@return		UIButtonBase*
 */
UIButtonBase* MenuSelectAction::FindButtonByName(const std::string& buttonName) {
    auto itr = buttonMap.find(buttonName);
    if (itr == buttonMap.end()) return nullptr;

    return itr->second;
}