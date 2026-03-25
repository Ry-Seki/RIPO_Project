/*
 *	@file	MenuInGame.cpp
 *	@author	Seki
 */

#include "MenuInGame.h"
#include "../MenuManager.h"
#include "../System/MenuVolumeSettings.h"
#include "../System/MenuSaveMode.h"
#include "../System/MenuLoadMode.h"
#include "../../UI/Button/SinglePressButton.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Load/LoadManager.h"
#include "../../Input/InputUtility.h"
#include "../../Engine.h"
#include "../../Scene/TitleScene.h"
#include "../MenuResourcesFactory.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Menu/System/MenuConfirm.h"
#include "../../Audio/AudioUtility.h"
#include "../../Load/Audio/LoadAudio.h"

namespace {
    /*
     *  @brief  ファイルパス
     */
    constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/InGame/InGameMenuResources.json";
    constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/InGame/InGameMenuNavigation.json";
    constexpr const char* _MENU_BUTTON_DATA_PATH = "Data/UI/MainGame/InGame/InGameMenuButtonData.json";

    // 別名定義
    using InGameMenuType = GameEnum::InGameMenuType;
    // 関数ポインタの別名定義
    using ExecuteMenuButton = void(MenuInGame::*)(Engine&);

    // メニュー処理の関数テーブル
    const std::array<ExecuteMenuButton, static_cast<int>(InGameMenuType::Max)> execute = {
        &MenuInGame::ExecuteLoadMenu,
        &MenuInGame::ExecuteSaveMenu,
        &MenuInGame::ExecuteSettingsMenu,
        &MenuInGame::ExecuteReturnTitle,
        &MenuInGame::ExecuteReturnPrevMenu
    };
    /*
     *  @brief  インゲームメニューの種類マップ
     */
    const std::unordered_map<std::string, InGameMenuType> inGameMenuMap = {
        { "Load", InGameMenuType::Load },
        { "Save", InGameMenuType::Save },
        { "Settings", InGameMenuType::Settings },
        { "ReturnTitle", InGameMenuType::ReturnTitle },
        { "Back", InGameMenuType::Back }
    };
    /*
     *  @brief  メニューボタンデータ構造体
     */
    struct MenuButtonData {
        std::string name = "";
        InGameMenuType type = InGameMenuType::Invalid;
    };
    /*
     *	@brief	    メニューの種類識別
     *  @param[in]  const std::string& typeKey
     *  @return     InGameMenuType
     */
    InGameMenuType StringToMenuType(const std::string& typeKey) {
        auto itr = inGameMenuMap.find(typeKey);

        if (itr != inGameMenuMap.end()) return itr->second;

        return InGameMenuType::Invalid;
    }
    /*
     *  @brief      JSON->メニューボタン情報へ変換
     *  @param[in]  const JSON& json
     *  @return     std::vector<MenuButtonData>
     */
    std::vector<MenuButtonData> ParseMenuButtonData(const JSON& json) {
        std::vector<MenuButtonData> result;

        auto array = json["MenuButtons"];

        for (const auto& node : array) {
            MenuButtonData entry;

            entry.name = node["ButtonName"].get<std::string>();

            std::string typeString = node["MenuType"].get<std::string>();
            entry.type = StringToMenuType(typeString);

            result.push_back(entry);
        }
        return result;
    }
}
/*
 *	@brief	初期化処理
 */
void MenuInGame::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
    auto buttonData = load.LoadResource<LoadJSON>(_MENU_BUTTON_DATA_PATH);

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
        InitializeMenuButtons(buttonData->GetData(), engine);
        // イベントシステムのnavigationの設定
        eventSystem.LoadNavigation(navigation->GetData());
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuInGame::Open() {
    MenuBase::Open();
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 0.5f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        eventSystem.ApplySelection();
        InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    });
}
/*
 *	@brief	更新処理
 */
void MenuInGame::Update(Engine& engine, float unscaledDeltaTime) {
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
void MenuInGame::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
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
void MenuInGame::Render() {
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
 *	@brief	メニューを閉じる
 */
void MenuInGame::Close(Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief	メニューを中断
 */
void MenuInGame::Suspend() {
    MenuBase::Suspend();
}
/*
 *	@brief	メニューを再開
 */
void MenuInGame::Resume() {
    MenuBase::Resume();
    for (auto& button : buttonList) {
        button->Setup();
    }
    eventSystem.ApplySelection();
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	GameEnum::InGameMenuType type
 */
void MenuInGame::SelectButtonExecute(GameEnum::InGameMenuType type, Engine& engine) {
    AudioUtility::PlaySE("DebugSE");
    // タイトルに戻るボタン
    if (type == GameEnum::InGameMenuType::ReturnTitle) {
        ExecuteReturnTitle(engine);
        return;
    }
    StartFadeEndCallback(type, engine);
}
/*
 *	@brief		メニューボタンの初期化処理
 *	@param[in]	const JSON& json
 */
void MenuInGame::InitializeMenuButtons(const JSON& json, Engine& engine) {
    auto actionData = ParseMenuButtonData(json);

    for (const auto& entry : actionData) {
        UIButtonBase* button = FindButtonByName(entry.name);
        if (!button) continue;
        const auto type = entry.type;
        // ボタン実行処理の登録
        button->RegisterOnClick([this, type, &engine]() {
            SelectButtonExecute(type, engine);
        });
        // ボタンに navigation 更新処理を登録
        button->RegisterUpdateSelectButton([this, button]() {
            eventSystem.UpdateSelectButton(button);
        });
    }
}
/*
 *	@brief		フェード後->コールバックの実行処理
 *	@param[in]	GameEnum::InGameMenuType type
 */
void MenuInGame::StartFadeEndCallback(GameEnum::InGameMenuType type, Engine& engine) {
    isInteractive = false;
    FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeOut, [this, type, &engine]() {
        // アクションの実行
        (this->*execute[static_cast<int>(type)])(engine);
    });
}
/*
 *	@brief		名前でのボタン検索
 *	@param[in]	const std::string& buttonName
 *	@return		UIButtonBase*
 */
UIButtonBase* MenuInGame::FindButtonByName(const std::string& buttonName) {
    auto itr = buttonMap.find(buttonName);
    if (itr == buttonMap.end()) return nullptr;

    return itr->second;
}
/*
 *	@brief	ロード画面を開く
 */
void MenuInGame::ExecuteLoadMenu(Engine& engine) {
    auto& menu = MenuManager::GetInstance();
    menu.OpenMenu<MenuLoadMode>();
}
/*
 *	@brief	セーブ画面を開く
 */
void MenuInGame::ExecuteSaveMenu(Engine& engine) {
    auto& menu = MenuManager::GetInstance();
    menu.OpenMenu<MenuSaveMode>();
}
/*
 *	@brief	設定画面を開く
 */
void MenuInGame::ExecuteSettingsMenu(Engine& engine) {
    auto& menu = MenuManager::GetInstance();
    menu.OpenMenu<MenuVolumeSettings>();
}
/*
 *	@brief	タイトル画面に戻る
 */
void MenuInGame::ExecuteReturnTitle(Engine& engine) {
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();
    confirm->SetCallback([&engine, this, &menu](GameEnum::ConfirmResult result) {
        menu.CloseTopMenu();    // 確認メニュー
        if (result != GameEnum::ConfirmResult::Yes) return;
        isInteractive = false;
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, &engine]() {
            menu.CloseAllMenu();    // 全てのメニュー
            engine.SetNextScene(std::make_shared<TitleScene>());
        });
    });
    menu.OpenMenu<MenuConfirm>();
}
/*
 *	@brief	前の画面に戻る
 */
void MenuInGame::ExecuteReturnPrevMenu(Engine& engine) {
    auto& menu = MenuManager::GetInstance();
    menu.CloseTopMenu();    // このメニュー
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 0.5f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn);
}