/*
 *	@file	MenuSelectShopItem.cpp
 *	@author	Seki
 */

#include "MenuSelectShopItem.h"
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
#include "MenuPurchaseCount.h"
#include "../../../Manager/WeaponManager.h"
#include "../../../Audio/AudioUtility.h"
#include "../../../Manager/FontManager.h"

/*
 *	@brief	初期化処理
 */
void MenuSelectShopItem::Initialize(Engine& engine) {
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
        for (auto& button : buttonList) {
            if (button->GetName() == "SubmachineGun") smgWeapon = button.get();
            else if (button->GetName() == "Exit") exitButton = button.get();
            else if (button->GetName() == "Back") backButton = button.get();
        }
        eventSystem.LoadNavigation(navigation->GetData());
    });
}
/* 
 *	@brief	メニューを開く
 */
void MenuSelectShopItem::Open() {
    MenuBase::Open();
    auto& money = MoneyManager::GetInstance();
    currentSlot = -1;
    currentMoney = money.GetCurrentMoney();
    catalogData = money.GetItemCatalogData();

    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    for (auto& button : buttonList) {
        button->Setup();
    }
    // すでにSMGを解放していたら、選択不可能状態にする
    if (WeaponManager::GetInstance().IsSubmachineGun()) smgWeapon->SetIsEnable(false);

    if (IsBuyItem()) {
        backButton->SetIsEnable(false);
    }
    else {
        exitButton->SetIsEnable(false);
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
void MenuSelectShopItem::Update(Engine& engine, float unscaledDeltaTime) {
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

    if (!inputHandle && input.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]) {
        inputHandle = true;
        button->OnPressDown();
    }
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuSelectShopItem::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
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
    currentSlot = eventSystem.GetCurrentIndex();
}
/*
 *	@brief	描画処理
 */
void MenuSelectShopItem::Render() {
    auto& font = FontManager::GetInstance();

    for (auto& sprite : spriteList) {
        sprite->Render();
    }
    for (int i = buttonList.size() - 1; i >= 0; i--) {
        buttonList[i]->Render();
    }
    ItemData* item;
    if (bool isGetItem = catalogData.TryGetItem(currentSlot, item)) {
        std::string itemPrice = std::to_string(item->price);
        font.Draw("BuyItem", 1502, 289, itemPrice, GetColor(255, 255, 255));
    }
    std::string money = std::to_string(MoneyManager::GetInstance().GetCurrentMoney());
    font.Draw("NormalSizeFont", 1611, 830, money, GetColor(75, 75, 75));

}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectShopItem::Close(Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief	メニューを再開
 */
void MenuSelectShopItem::Resume() {
    MenuBase::Resume();
    currentMoney = MoneyManager::GetInstance().GetCurrentMoney();
    for (auto& button : buttonList) {
        button->Setup();
    }
    if (WeaponManager::GetInstance().IsSubmachineGun()
        && smgWeapon->IsEnable()) {
        smgWeapon->SetIsEnable(false);
    }
    if (IsBuyItem()) {
        backButton->SetIsEnable(false);
    }else {
        exitButton->SetIsEnable(false);
    }

    eventSystem.ApplySelection();
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuSelectShopItem::SelectButtonExecute(Engine& engine, int buttonIndex) {
    int itemID = buttonIndex;
    auto& menu = MenuManager::GetInstance();
    ItemData* item;
    GameEnum::ShopActionType type = GameEnum::ShopActionType::Invalid;

    if (bool isGetItem = catalogData.TryGetItem(currentSlot, item)) {
        AudioUtility::PlaySE("DebugSE");
        auto purchase = menu.GetMenu<MenuPurchaseCount>();
        purchase->SetItemData(item);
        menu.OpenMenu<MenuPurchaseCount>();
    } else {
        if (buttonIndex == buttonList.size() - 1) {
            type = GameEnum::ShopActionType::Back;
        } else {
            type = GameEnum::ShopActionType::Exit;
        }

        AudioUtility::PlaySE("DebugSE");
        auto confirm = menu.GetMenu<MenuConfirm>();
        confirm->SetCallback([this, &menu, type](GameEnum::ConfirmResult result) {
            if (result == GameEnum::ConfirmResult::Yes) {
                FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
                FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, type]() {
                    menu.CloseAllMenu();
                    if (Callback) Callback(type);
                });
            } else {
                menu.CloseTopMenu();
            }
        });
        menu.OpenMenu<MenuConfirm>();
    }
}