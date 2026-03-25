/*
 *	@file	MenuMoneyChange.cpp
 *	@author	Seki
 */

#include "MenuMoneyChange.h"
#include "../../../Audio/AudioUtility.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../Load/Audio/LoadAudio.h"
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
#include "../../../Random.h"
#include "../../../Manager/FontManager.h"

/*
 *  @brief  定数の名前空間
 */
namespace {
    constexpr float _CHANGE_DURATION = 1.0f;
    constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Money/ChangeMoneyMenuResources.json";
    constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/Money/ChangeMoneyMenuNavigation.json";
    constexpr const char* _GET_MONEY_SE_PATH = "Res/Audio/SE/GetMoney.mp3";
}

/*
 *	@brief	初期化処理
 */
void MenuMoneyChange::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
    auto getSE = load.LoadResource<LoadAudio>(_GET_MONEY_SE_PATH);

    load.SetOnComplete([this, &engine, menuJSON, navigation, getSE]() {
        // メニューUI生成
        MenuInfo result = MenuResourcesFactory::Create(menuJSON->GetData());
        // メニューUIの所有権移動
        spriteList = std::move(result.spriteList);
        textList = std::move(result.textList);
        buttonList = std::move(result.buttonList);
        // ボタンの登録
        for (const auto& button : buttonList) {
            if (!button) continue;
            // ボタンの登録
            eventSystem.RegisterButton(button.get());
            // ボタン実行処理の登録
            button->RegisterOnClick([this]() {
                SelectButtonExecute();
            });
            // ボタンに navigation 更新処理を登録
            button->RegisterUpdateSelectButton([this, button]() {
                eventSystem.UpdateSelectButton(button.get());
            });
        }
        // イベントシステムの初期化
        eventSystem.Initialize(0);
        // イベントシステムのnavigationの設定
        eventSystem.LoadNavigation(navigation->GetData());
        // SEの登録
        AudioUtility::RegisterSEHandle("GetMoney", getSE->GetHandle());
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuMoneyChange::Open() {
    MenuBase::Open();
    const int white = GetColor(255, 255, 255);
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    for (auto& text : textList) {
        text->SetColor(white);
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
void MenuMoneyChange::Update(Engine& engine, float unscaledDeltaTime) {
    auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

    // イベントシステムの更新
    eventSystem.Update(unscaledDeltaTime);
    // アニメーション処理
    if (isAnimationEnd) {
        // ボタンの更新
        for (auto& button : buttonList) {
            if (!button) continue;

            button->Update(unscaledDeltaTime);
        }
        // 現在選択されているボタンの取得
        auto button = eventSystem.GetCurrentSelectButton();
        if (!button) return;

        if (input.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]) {
            button->OnPressDown();
        }
    } else {
        isAnimationEnd = ShowChangeMoney(unscaledDeltaTime);
    }
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuMoneyChange::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
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
    currentIndex = eventSystem.GetCurrentIndex();
}
/*
 *	@brief	描画処理
 */
void MenuMoneyChange::Render() {
    auto& font = FontManager::GetInstance();
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible()) continue;
        sprite->Render();
    }
    for (auto& button : buttonList) {
        if (!button->IsVisible()) continue;
        button->Render();
    }
    std::string showMoneyStr = std::to_string(randomMoney);
    std::string changeMoneyStr = std::to_string(changeMoney);

    textList[0]->SetText(showMoneyStr);
    textList[1]->SetText(changeMoneyStr);

    textList[0]->Render();
    if(isAnimationEnd) textList[1]->Render();
}
/*
 *	@brief	メニューを閉じる
 */
void MenuMoneyChange::Close(Engine& engine) {
    MenuBase::Close(engine);
    currentMoney = 0;
    prevMoney = 0;
    changeMoney = 0;
    randomMoney = 0;
    elapsedTime = 0.0f;
    isAnimationEnd = false;
}
/*
 *	@brief	メニューを再開
 */
void MenuMoneyChange::Resume() {
    MenuBase::Resume();
    for (auto& button : buttonList) {
        button->Setup();
    }
}
/*
 *	@brief		ボタンの押された時の処理
 */
void MenuMoneyChange::SelectButtonExecute() {
    auto& menu = MenuManager::GetInstance();
    AudioUtility::PlaySE("DebugSE");
    isInteractive = false;
    FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
        menu.CloseAllMenu();
        if (Callback) Callback();
    });
}
/*
 *	@brief		収支の演出
 *	@return		bool
 */
bool MenuMoneyChange::ShowChangeMoney(float unscaledDeltaTime) {
    if (isAnimationEnd) return true;
    changeMoney = currentMoney - prevMoney;
    // 収支に変動がなければ即終了
    if (changeMoney == 0) {
        randomMoney = currentMoney;
        return true;
    }
    // 現在のお金の桁を取得
    int minDigitValue = 0;
    int maxDigitValue = 0;
    GetDigitMoney(minDigitValue, maxDigitValue);

    // 桁に合わせたお金の変動アニメーション(ランダム,桁数は合わせる, 秒数指定)
    elapsedTime += unscaledDeltaTime;
    float ratio = elapsedTime / _CHANGE_DURATION;
    if (ratio < 0.9f) {
        // Random::Range は float を返すので int にキャスト
        randomMoney = static_cast<int>(
            Random::Range(
                static_cast<float>(minDigitValue),
                static_cast<float>(maxDigitValue + 1))
            );

        return false;
    }

    // 確定
    randomMoney = currentMoney;

    if (elapsedTime > _CHANGE_DURATION) {
        elapsedTime = 0.0f;
        AudioUtility::PlaySE("GetMoney");
        return true;
    }

    return false; 
}
/*
 *	@brief		現在のお金の桁が同じ数字の最小数と最大数を返す
 *	@param[out] int& minDigitValue
 *	@param[out]	int& maxDigitValue
 */
void MenuMoneyChange::GetDigitMoney(int& minDigitValue, int& maxDigitValue) {
    int digit = 0;
    int value = currentMoney;

    while (value > 0) {
        value /= 10;
        digit++;
    }

    minDigitValue = 1;
    for (int i = 1; i < digit; i++) {
        minDigitValue *= 10;
    }
    maxDigitValue = minDigitValue * 10 - 1;
}

