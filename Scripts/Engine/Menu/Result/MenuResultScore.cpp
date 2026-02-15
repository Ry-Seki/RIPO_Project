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
#include "../../Manager/FontManager.h"
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
        spriteList = std::move(result.spriteList);
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
    // ボタンの準備処理
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
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
 *	@brief	アニメーション等の更新
 */
void MenuResultScore::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
    animTimer += unscaledDeltaTime;

    if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
    animTimer -= GameConst::UI_ANIM_INTERVAL;

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
void MenuResultScore::Render() {
    auto& money = MoneyManager::GetInstance();
    auto& font = FontManager::GetInstance();
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible()) continue;
        sprite->Render();
    }
    for (auto& button : buttonList) {
        if (!button->IsVisible()) continue;
        button->Render();
    }
    std::string showMoneyStr = std::to_string(money.GetCurrentMoney());
    std::string scoreRank = ToRankString();
    font.Draw("NormalSizeFont", 1550, 490, showMoneyStr, GetColor(255, 255, 255));
    font.Draw("NormalSizeFont", 1550, 638, scoreRank, GetColor(255, 255, 255));

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
/*
 *	@brief		ランクから->文字列型に変換
 *	@return		std::string
 */
std::string MenuResultScore::ToRankString() {
    std::string rankStr;
    switch (rank) {
        case GameEnum::ResultRank::S:
            rankStr = 'S';
            break;
        case GameEnum::ResultRank::A:
            rankStr = 'A';
            break;
        case GameEnum::ResultRank::B:
            rankStr = 'B';
            break;
        case GameEnum::ResultRank::C:
            rankStr = 'C';
            break;
        case GameEnum::ResultRank::D:
            rankStr = 'D';
            break;
    }
    return rankStr;
}
