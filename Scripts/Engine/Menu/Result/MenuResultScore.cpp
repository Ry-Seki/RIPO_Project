/*
 *	@file	MenuResultScore.cpp
 *	@author	Seki
 */

#include "MenuResultScore.h"
#include "../MenuResourcesFactory.h"
#include "../MenuManager.h"
#include "../System/MenuConfirm.h"
#include "../../Fade/FadeManager.h"
#include "../../Fade/FadeFactory.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../System/Money/MoneyManager.h"
#include "../../Input/InputUtility.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Scene/TitleScene.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void MenuResultScore::Initialize(Engine& engine) {
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
void MenuResultScore::Open() {
    MenuBase::Open();
    int playerScore = MoneyManager::GetInstance().GetCurrentMoney();
    rank = JudgeRank(playerScore);
    currentSlot = -1;
    // ボタンの準備処理
    for (auto& button : buttonList) {
        button->Setup();
    }
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        isStart = true;
        eventSystem.ApplySelection();
        InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    });

}
/*
 *	@brief	更新処理
 */
void MenuResultScore::Update(Engine& engine, float unscaledDeltaTime) {
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
 *	@brief	描画処理
 */
void MenuResultScore::Render() {
    if (!isStart) return;

    MoneyManager& money = MoneyManager::GetInstance();
    DrawFormatString(50, 50, GetColor(255, 255, 255), "ResultMoney : %d", money.GetCurrentMoney());
    DrawFormatString(50, 70, GetColor(255, 255, 255), "ResultScore ");
    // TODO : それぞれの描画インターフェーズでの描画にする
    switch (rank) {
        case GameEnum::ResultRank::Invalid:
            DrawFormatString(200, 70, GetColor(255, 255, 255), "無効");
            break;
        case GameEnum::ResultRank::S:
            DrawFormatString(200, 70, GetColor(255, 255, 255), "S ");
            break;
        case GameEnum::ResultRank::A:
            DrawFormatString(200, 70, GetColor(255, 255, 255), "A ");
            break;
        case GameEnum::ResultRank::B:
            DrawFormatString(200, 70, GetColor(255, 255, 255), "B ");
            break;
        case GameEnum::ResultRank::C:
            DrawFormatString(200, 70, GetColor(255, 255, 255), "C ");
            break;
        case GameEnum::ResultRank::D:
            DrawFormatString(200, 70, GetColor(255, 255, 255), "D ");
            break;
    }
    for (auto& button : buttonList) {
        button->Render();
    }
}
/*
 *	@brief	メニューを閉じる
 */
void MenuResultScore::Close(Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief	メニューを閉じる
 */
void MenuResultScore::Suspend() {
    MenuBase::Suspend();
}
/*
 *	@brief	メニューを再開
 */
void MenuResultScore::Resume() {
    MenuBase::Resume();
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuResultScore::SelectButtonExecute(Engine& engine, int buttonIndex) {
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();
    confirm->SetCallback([this, &menu, &engine](GameEnum::ConfirmResult result) {
        if (result == GameEnum::ConfirmResult::Yes) {
            FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
            FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, &engine]() {
                // TODO : お金のリセット
                menu.CloseAllMenu();
                engine.SetNextScene(std::make_shared<TitleScene>());
            });
        } else {
            menu.CloseTopMenu();
        }
    });
    menu.OpenMenu<MenuConfirm>();
}
/*
 *	@brief		スコア->ランク判定
 *	@param[in]	int playerScore
 *  @return     GameEnum::ResultRank
 */
GameEnum::ResultRank MenuResultScore::JudgeRank(int playerScore) {
    for (int i = 0, max = static_cast<int>(GameEnum::ResultRank::Max); i < max; ++i) {
        if (playerScore >= resultScore.requiredScore[i]) {
            return static_cast<GameEnum::ResultRank>(i);
        }
    }
    return GameEnum::ResultRank::Invalid;
}
