/*
 *	@file	MenuConfirm.cpp
 *	@author	Seki
 */

#include "MenuConfirm.h"
#include "../../Audio/AudioUtility.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Input/InputUtility.h"
#include "../../Save/SaveDataManager.h"
#include "../../UI/Button/SinglePressButton.h"
#include "../../Engine.h"
#include "../../Scene/MainGameScene.h"
#include "../MenuResourcesFactory.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Menu/MenuManager.h"

#include <DxLib.h>

namespace {
    /*
     *  @brief  ファイルパス
     */
    constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/System/Confirm/ConfirmResources.json";
    constexpr const char* _NAVIGATION_PATH = "Data/UI/System/Confirm/ConfirmNavigation.json";
    constexpr const char* _CONFIRM_BUTTON_DATA_PATH = "Data/UI/System/Confirm/ConfirmButtonData.json";

    // 別名定義
    using ConfirmResult = GameEnum::ConfirmResult;

    /*
     *  @brief  確認の種類マップ
     */
    const std::unordered_map<std::string, ConfirmResult> confirmResultMap = {
        { "Yes", ConfirmResult::Yes },
        { "No", ConfirmResult::No }
    };
    /*
     *  @brief  確認ボタン構造体
     */
    struct ConfirmButtonData {
        std::string name = "";
        ConfirmResult result = ConfirmResult::Invalid;
    };
    /*
     *	@brief	    確認結果の種類識別
     *  @param[in]  const std::string& typeKey
     *  @return     ConfirmResult
     */
    ConfirmResult StringToConfirmResult(const std::string& typeKey) {
        auto itr = confirmResultMap.find(typeKey);

        if (itr != confirmResultMap.end()) return itr->second;

        return ConfirmResult::No;
    }
    /*
     *  @brief      JSON->確認ボタン情報へ変換
     *  @param[in]  const JSON& json
     *  @return     std::vector<ConfirmButtonData>
     */
    std::vector<ConfirmButtonData> ParseTrainingButtonData(const JSON& json) {
        std::vector<ConfirmButtonData> result;

        auto array = json["ConfirmButtons"];

        for (const auto& node : array) {
            ConfirmButtonData entry;

            entry.name = node["ButtonName"].get<std::string>();

            std::string typeString = node["ConfirmResult"].get<std::string>();
            entry.result = StringToConfirmResult(typeString);

            result.push_back(entry);
        }
        return result;
    }
}
/*
 *	@brief	初期化処理
 */
void MenuConfirm::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
    auto buttonData = load.LoadResource<LoadJSON>(_CONFIRM_BUTTON_DATA_PATH);

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
        SetupConfirmButtons(buttonData->GetData());
        // イベントシステムのnavigationの設定
        eventSystem.LoadNavigation(navigation->GetData());
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuConfirm::Open() {
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
void MenuConfirm::Update(Engine& engine, float unscaledDeltaTime) {
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
void MenuConfirm::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
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
void MenuConfirm::Render() {
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    DrawBox(0, 0, GameConst::WINDOW_WIDTH, GameConst::WINDOW_HEIGHT, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

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
void MenuConfirm::Close(Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief	メニューを中断
 */
void MenuConfirm::Suspend() {
    MenuBase::Suspend();
}
/*
 *	@brief	メニューを再開
 */
void MenuConfirm::Resume() {
    MenuBase::Resume();
    for (auto& button : buttonList) {
        button->Setup();
    }
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	GameEnum::ConfirmResult result
 */
void MenuConfirm::SelectButtonExecute(GameEnum::ConfirmResult result) {
    AudioUtility::PlaySE("DebugSE");
    isInteractive = false;
    if (Callback) Callback(result);
    Callback = nullptr;
}
/*
 *	@brief		確認ボタンの準備前処理
 *	@param[in]	const JSON& json
 */
void MenuConfirm::SetupConfirmButtons(const JSON& json) {
    auto confirmData = ParseTrainingButtonData(json);

    for (const auto& entry : confirmData) {
        UIButtonBase* button = FindButtonByName(entry.name);
        if (!button) continue;
        const auto result = entry.result;
        // ボタン実行処理の登録
        button->RegisterOnClick([this, result]() {
            SelectButtonExecute(result);
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
UIButtonBase* MenuConfirm::FindButtonByName(const std::string& buttonName) {
    auto itr = buttonMap.find(buttonName);
    if (itr == buttonMap.end()) return nullptr;

    return itr->second;
}