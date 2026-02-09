/*
 *	@file	MenuPurchaseCount.cpp
 *	@author	Seki
 */
#include "MenuPurchaseCount.h"
#include "../MenuManager.h"
#include "../System/MenuSettings.h"
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

/*
 *	@brief	初期化処理
 */
void MenuPurchaseCount::Initialize(Engine& engine) {
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
        spriteList = std::move(result.spriteList);
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
void MenuPurchaseCount::Open() {
    MenuBase::Open();
    currentSlot = -1;
    InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    for (auto& button : buttonList) {
        button->Setup();
    }
    eventSystem.ApplySelection();
}
/*
 *	@brief	更新処理
 */
void MenuPurchaseCount::Update(Engine& engine, float unscaledDeltaTime) {
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
void MenuPurchaseCount::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
    animTimer += unscaledDeltaTime;

    if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
    animTimer -= GameConst::UI_ANIM_INTERVAL;

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
void MenuPurchaseCount::Render() {
    for (auto& sprite : spriteList) {
        sprite->Render();
    }
    for (auto& button : buttonList) {
        button->Render();
    }
}
/*
 *	@brief	メニューを閉じる
 */
void MenuPurchaseCount::Close(Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief	メニューを中断
 */
void MenuPurchaseCount::Suspend() {
    MenuBase::Suspend();
}
/*
 *	@brief	メニューを再開
 */
void MenuPurchaseCount::Resume() {
    MenuBase::Resume();
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuPurchaseCount::SelectButtonExecute(Engine& engine, int buttonIndex) {
}
/*
 *	@brief		購入個数の設定
 */
void MenuPurchaseCount::SetPurchaseCount() {
}
/*
 *	@brief		アイテムの購入個数を一つ増加
 */
void MenuPurchaseCount::AddPurchaseCount() {
    // 現在の所持金を取得

}
/*
 *	@brief		アイテムの購入個数を一つ減少
 */
void MenuPurchaseCount::SubPurchaseCount() {
}