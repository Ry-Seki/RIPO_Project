/*
 *	@file	MenuPurchaseCount.cpp
 *	@author	Seki
 */
#include "MenuPurchaseCount.h"
#include "../../MenuManager.h"
#include "../../System/MenuVolumeSettings.h"
#include "../../System/MenuSaveMode.h"
#include "../../System/MenuLoadMode.h"
#include "../../../UI/Button/SinglePressButton.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../Load/Audio/LoadAudio.h"
#include "../../../Load/LoadManager.h"
#include "../../../Input/InputUtility.h"
#include "../../../Engine.h"
#include "../../../Scene/TitleScene.h"
#include "../../MenuResourcesFactory.h"
#include "../../../../Data/UI/MenuInfo.h"
#include "../../../Menu/System/MenuConfirm.h"
#include "../../../System/Money/MoneyManager.h"
#include "../../../../Data/ItemCatalogData.h"
#include "../../../Manager/FontManager.h"
#include "../../../Audio/AudioUtility.h"

namespace {
    /*
     *  @brief  ファイルパス
     */
    constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Shop/PurchaseCount/PurchaseMenuResources.json";
    constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/Shop/PurchaseCount/PurchaseMenuNavigation.json";
    constexpr const char* _PURCHASE_BUTTON_DATA_PATH = "Data/UI/MainGame/Shop/PurchaseCount/PurchaseButtonData.json";
    constexpr const char* _BUY_SE_PATH = "Res/Audio/SE/BuyItem.mp3";

    // 別名定義
    using PurchaseButtonType = MenuPurchaseCount::PurchaseButtonType;
    
    /*
     *  @brief  購入ボタンの種類マップ
     */
    const std::unordered_map<std::string, PurchaseButtonType> purchaseTypeButton = {
        { "AddCount", PurchaseButtonType::AddButton },
        { "SubCount", PurchaseButtonType::SubButton },
        { "Buy", PurchaseButtonType::BuyButton },
        { "Cancel", PurchaseButtonType::CancelButton }
    };
    /*
     *  @brief  購入ボタン構造体
     */
    struct PurchaseButtonData {
        std::string name = "";
        PurchaseButtonType type = PurchaseButtonType::Invalid;
    };
    /*
     *  @brief      購入ボタンの種類識別
     *  @param[in]  const std::string& typeKey
     *  @return     PurchaseButtonType
     */
    PurchaseButtonType StringToPurchaseButtonType(const std::string& typeKey) {
        auto itr = purchaseTypeButton.find(typeKey);

        if (itr != purchaseTypeButton.end()) return itr->second;

        return PurchaseButtonType::Invalid;
    }
    /*
     *  @brief      JSON->購入ボタン情報へ変換
     *  @param[in]  const JSON& json
     */
    std::vector<PurchaseButtonData> ParsePurchaseButtonData(const JSON& json) {
        std::vector<PurchaseButtonData> result;

        auto array = json["PurchaseButtons"];

        for (const auto& node : array) {
            PurchaseButtonData entry;

            entry.name = node["ButtonName"].get<std::string>();

            std::string typeString = node["PurchaseButtonType"].get<std::string>();
            entry.type = StringToPurchaseButtonType(typeString);

            result.push_back(entry);
        }
        return result;
    }
}

/*
 *	@brief	初期化処理
 */
void MenuPurchaseCount::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
    auto buttonData = load.LoadResource<LoadJSON>(_PURCHASE_BUTTON_DATA_PATH);
    auto buySE = load.LoadResource<LoadAudio>(_BUY_SE_PATH);

    load.SetOnComplete([this, &engine, menuJSON, navigation, buttonData, buySE]() {
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
        // イベントシステムの初期化
        eventSystem.Initialize(0);
        // ボタンの準備前処理
        SetupPurchaseButtons(buttonData->GetData());
        // イベントシステムのnavigationの設定
        eventSystem.LoadNavigation(navigation->GetData());
        // SEの登録
        AudioUtility::RegisterSEHandle("BuySE", buySE->GetHandle());
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuPurchaseCount::Open() {
    MenuBase::Open();
    purchaseCount = 0;
    purchaseMoney = 0;
    const int white = GetColor(255, 255, 255);
    auto& money = MoneyManager::GetInstance();
    // アイテムデータの設定
    currentMoney = money.GetCurrentMoney();
    leastMoney = currentMoney;
    for (auto& button : buttonList) {
        button->Setup();
    }
    for (auto& text : textList) {
        text->SetColor(white);
    }
    // 購入ボタンの制限
    if (currentMoney <= 0) {
        auto button = FindButtonByName("Buy");
        if (button) button->SetIsEnable(false);
    }
    eventSystem.ApplySelection();
    InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
}
/*
 *	@brief	更新処理
 */
void MenuPurchaseCount::Update(Engine& engine, float unscaledDeltaTime) {
    auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

    auto buyButton = FindButtonByName("Buy");
    if (buyButton) {
        // 購入数が0の時は購入ボタンの操作可能フラグをfalseにする
        if (buyButton->IsEnable()) {
            if (purchaseCount <= 0) buyButton->SetIsEnable(false);
        }
        else {
            if (purchaseCount > 0) buyButton->SetIsEnable(true);
        }
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
void MenuPurchaseCount::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
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
    // テキストの更新
    std::string textStringList[3] = {
    std::to_string(leastMoney),
    std::to_string(purchaseCount),
    std::to_string(purchaseMoney)
    };
    // テキストのセット
    for (int i = 0; i < 3; i++) {
        auto& text = textList[i];
        if (!text) continue;

        text->SetText(textStringList[i]);
    }

}
/*
 *	@brief	描画処理
 */
void MenuPurchaseCount::Render() {
    auto& font = FontManager::GetInstance();

    for (auto& sprite : spriteList) {
        sprite->Render();
    }
    for (auto& button : buttonList) {
        button->Render();
    }
    for (auto& text : textList) {
        text->Render();
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
 *	@brief	メニューを再開
 */
void MenuPurchaseCount::Resume() {
    MenuBase::Resume();
    for (auto& button : buttonList) {
        button->Setup();
    }
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	PurchaseButtonType type
 */
void MenuPurchaseCount::SelectButtonExecute(MenuPurchaseCount::PurchaseButtonType type) {
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();
    AudioUtility::PlaySE("DebugSE");

    switch (type) {
        case MenuPurchaseCount::PurchaseButtonType::AddButton:
            AddPurchaseCount();
            break;
        case MenuPurchaseCount::PurchaseButtonType::SubButton:
            SubPurchaseCount();
            break;
        case MenuPurchaseCount::PurchaseButtonType::BuyButton:{
            confirm->SetCallback([this, &menu](GameEnum::ConfirmResult result) {
                AudioUtility::PlaySE("DebugSE");
                menu.CloseTopMenu();        // 確認メニュー
                if (result != GameEnum::ConfirmResult::Yes) return;
                // 購入処理
                ConfirmPurchaseItem();
                // このメニューを閉じる
                menu.CloseTopMenu();    // このメニュー
            });
            menu.OpenMenu<MenuConfirm>();
        }
            break;
        case MenuPurchaseCount::PurchaseButtonType::CancelButton:
            menu.CloseTopMenu();
            break;
        default:
            break;
    }
}
/*
 *	@brief		購入の確定
 */
void MenuPurchaseCount::ConfirmPurchaseItem() {
    AudioUtility::PlaySE("BuyItem");
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

    if (currentMoney < price) return;

    // 最大購入数の取得
    int maxCount = 0;
    if (targetItemData->buyCount) {
        maxCount = 1;
    }else {
       maxCount = currentMoney / price;
    }
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
    leastMoney = currentMoney - purchaseMoney;
}
/*
 *	@brief		アイテムの購入個数を一つ減少
 */
void MenuPurchaseCount::SubPurchaseCount() {
    if (!targetItemData) return;
    if (currentMoney <= 0) return;

    const int price = targetItemData->price;

    if (currentMoney < price) return;

    // 最大購入数の取得
    int maxCount = 0;
    if (targetItemData->buyCount) {
        maxCount = 1;
    }else {
        maxCount = currentMoney / price;
    }

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
    leastMoney = currentMoney - purchaseMoney;
}
/*
 *	@brief		購入ボタンの準備前処理
 *	@param[in]	const JSON& json
 */
void MenuPurchaseCount::SetupPurchaseButtons(const JSON& json) {
    auto purchaseData = ParsePurchaseButtonData(json);

    for (const auto& entry : purchaseData) {
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
 *	@brief		名前でのボタン検索
 *	@param[in]	const std::string& buttonName
 *	@return		UIButtonBase*
 */
UIButtonBase* MenuPurchaseCount::FindButtonByName(const std::string& buttonName) {
    auto itr = buttonMap.find(buttonName);
    if (itr == buttonMap.end()) return nullptr;

    return itr->second;
}