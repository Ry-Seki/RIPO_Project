/*
 *	@file	MenuPurchaseCount.cpp
 *	@author	Seki
 */
#include "MenuPurchaseCount.h"
#include "../../MenuManager.h"
#include "../../System/MenuSettings.h"
#include "../../System/MenuSaveMode.h"
#include "../../System/MenuLoadMode.h"
#include "../../../UI/Button/SinglePressButton.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../Load/LoadManager.h"
#include "../../../Input/InputUtility.h"
#include "../../../Engine.h"
#include "../../../Scene/TitleScene.h"
#include "../../MenuResourcesFactory.h"
#include "../../../../Data/UI/MenuInfo.h"
#include "../../../Menu/System/MenuConfirm.h"
#include "../../../System/Money/MoneyManager.h"
#include "../../../../Data/ItemCatalogData.h"

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
    auto& money = MoneyManager::GetInstance();
    // アイテムデータの設定
    currentMoney = money.GetCurrentMoney();
    for (auto& button : buttonList) {
        button->Setup();
    }
    eventSystem.ApplySelection();
    InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
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
    purchaseCount = 0;
    purchaseMoney = 0;
    targetItemData = nullptr;
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
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();

    if (buttonIndex == 0) {
        SubPurchaseCount();
    }
    else if (buttonIndex == 1) {
        AddPurchaseCount();
    }
    else if (buttonIndex == 2) {
        confirm->SetCallback([this, &menu](GameEnum::ConfirmResult result) {
            if (result == GameEnum::ConfirmResult::Yes) {
                // 購入処理
                ConfirmPurchaseItem();
                // メニューを閉じる
                menu.CloseTopMenu();
            }
            // 二回閉じる必要があるためここでも閉じる
            menu.CloseTopMenu();
        });
    }
    else if (buttonIndex == 3) {
        // 自身を閉じる
        menu.CloseTopMenu();
    }
}
/*
 *	@brief		購入の確定
 */
void MenuPurchaseCount::ConfirmPurchaseItem() {
    MoneyManager::GetInstance().SubMoney(purchaseMoney);
    if (!targetItemData) return;

    int targetID = targetItemData->itenID;
    if (Callback) Callback(targetID, purchaseCount);
}
/*
 *	@brief		アイテムの購入個数を一つ増加
 */
void MenuPurchaseCount::AddPurchaseCount() {
    if (!targetItemData) return;
    if (currentMoney <= 0) return;

    const int price = targetItemData->price;
    if (price <= 0) return;

    // 最大購入数の取得
    const int maxCount = currentMoney / price;
    if (maxCount <= 0) {
        purchaseCount = 0;
        purchaseMoney = 0;
        return;
    }
    // もし、現在の購入数が最大数の時は0にする
    if (purchaseCount >= maxCount) {
        purchaseCount = 0;
    } else {
        // 購入数を一つ増やす
        purchaseCount++;
    }
    // 購入金額
    purchaseMoney = price * purchaseCount;
}
/*
 *	@brief		アイテムの購入個数を一つ減少
 */
void MenuPurchaseCount::SubPurchaseCount() {
    if (!targetItemData) return;
    if (currentMoney <= 0) return;

    const int price = targetItemData->price;
    if (price <= 0) return;

    // 最大購入数の取得
    const int maxCount = currentMoney / price;
    if (maxCount <= 0) {
        purchaseCount = 0;
        purchaseMoney = 0;
        return;
    }
    // もし、現在の購入数が0の時は買える最大数にする
    if (purchaseCount <= 0) {
        purchaseCount = maxCount;
    }
    else {    
        // 購入数を一つ減らす
        purchaseCount--;
    }
    // 購入金額
    purchaseMoney = price * purchaseCount;
}