/*
 *	@file	MenuSelectShopItem.cpp
 *	@author	Seki
 */

#include "MenuSelectShopItem.h"
#include "../../MenuManager.h"
#include "../../System/MenuVolumeSettings.h"
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

namespace {
    /*
     *  @brief  ファイルパスの名前空間
     */
    constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Shop/SelectItem/SelectItemMenuResources.json";
    constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/Shop/SelectItem/SelectItemMenuNavigation.json";
    constexpr const char* _SHOP_ITEM_BUTTON_PATH = "Data/UI/MainGame/Shop/SelectItem/ShopButtonData.json";

    // 別名定義
    using ShopActionType = GameEnum::ShopActionType;

    /*
     *  @brief  ショップの種類マップ
     */
    const std::unordered_map<std::string, ShopActionType> shopActionTypeMap = {
        { "Buy", ShopActionType::Buy },
        { "Exit", ShopActionType::Exit },
        { "Back", ShopActionType::Back },
    };
    /*
     *  @brief  ショップボタン構造体
     */
    struct ShopButtonData {
        int itemID = -1;
        std::string name = "";
        ShopActionType type = ShopActionType::Invalid;
    };
    /*
     *  @brief      ショップ行動の種類識別
     *  @param[in]  const std::string& typeKey
     *  @return     ShopActionType
     */
    ShopActionType StringToShopAction(const std::string& typeKey) {
        auto itr = shopActionTypeMap.find(typeKey);

        if (itr != shopActionTypeMap.end()) return itr->second;

        return ShopActionType::Invalid;
    }
    /*
     *  @brief      JSON->ショップボタン情報へ変換
     *  @param[in]  const JSON& json
     *  @return     std::vector<ShopButtonData>
     */
    std::vector<ShopButtonData> ParseShopButtonData(const JSON& json) {
        std::vector<ShopButtonData> result;

        auto array = json["ShopButtons"];

        for (const auto& node : array) {
            ShopButtonData entry;

            entry.itemID = node["ItemID"].get<int>();
            entry.name = node["ButtonName"].get<std::string>();

            std::string typeString = node["ShopActionType"].get<std::string>();
            entry.type = StringToShopAction(typeString);

            result.push_back(entry);
        }
        return result;
    }
}
/*
 *	@brief	初期化処理
 */
void MenuSelectShopItem::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
    auto buttonData = load.LoadResource<LoadJSON>(_SHOP_ITEM_BUTTON_PATH);

    load.SetOnComplete([this, &engine, menuJSON, navigation, buttonData]() {
        // メニューUIの生成
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
        eventSystem.Initialize(0);
        // イベントシステムの初期化
        eventSystem.Initialize(0);
        // ボタンの準備前処理
        SetupShopButtons(buttonData->GetData());
        // イベントシステムのnavigationの設定
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
    UpdateButtonState();
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

        int animFrame = sprite->GetCurrentFrame();
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
        auto& button = buttonList[i];
        if (!button || !button->IsVisible()) continue;

        button->Render();
    }
    ItemData* item;
    if (bool isGetItem = catalogData.TryGetItem(currentSlot, item)) {
        std::string itemPrice = std::to_string(item->price);
        textList[0]->SetText(itemPrice);
        textList[0]->SetColor(GetColor(255, 255, 255));
        textList[0]->Render();
    }
    std::string money = std::to_string(MoneyManager::GetInstance().GetCurrentMoney());
    textList[1]->SetText(money);
    textList[1]->SetColor(GetColor(75, 75, 75));
    textList[1]->Render();
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
    UpdateButtonState();
    eventSystem.ApplySelection();
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	GameEnum::ShopActionType type
 *  @param[in]  int itemID
 */
void MenuSelectShopItem::SelectButtonExecute(GameEnum::ShopActionType type, int itemID) {
    auto& menu = MenuManager::GetInstance();
    AudioUtility::PlaySE("DebugSE");

    // ボタンの種類が買い物だった場合
    if (type == GameEnum::ShopActionType::Buy) {
        ItemData* item;
        // アイテムの取得
        if (!catalogData.TryGetItem(itemID, item)) return;
        // 購入個数のメニューを開く
        auto purchase = menu.GetMenu<MenuPurchaseCount>();
        purchase->SetItemData(item);
        menu.OpenMenu<MenuPurchaseCount>();
    } else if(type == GameEnum::ShopActionType::Back){
        menu.CloseTopMenu();	// このメニュー
        if (Callback) Callback(type);
    } else {
        // 確認メニューを開く
        OpenConfirmMenu(type);
    }
}
/*
 *	@brief		確認メニューを開く
 *	@param[in]	GameEnum::ShopActionType type
 */
void MenuSelectShopItem::OpenConfirmMenu(GameEnum::ShopActionType type) {
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();

    confirm->SetCallback([this, &menu, type](GameEnum::ConfirmResult result) {
        menu.CloseTopMenu();	// 確認メニュー
        if (result != GameEnum::ConfirmResult::Yes) return;
        isInteractive = false;
        // フェード後->コールバックの実行
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, type]() {
            menu.CloseTopMenu();	// このメニュー
            if (Callback) Callback(type);
        });
    });
    menu.OpenMenu<MenuConfirm>();
}
/*
 *	@brief		ショップボタンの準備前処理
 *	@param[in]	const JSON& json
 */
void MenuSelectShopItem::SetupShopButtons(const JSON& json) {
    auto shopData = ParseShopButtonData(json);

    for (const auto& entry : shopData) {
        UIButtonBase* button = FindButtonByName(entry.name);
        if (!button) continue;
        const auto type = entry.type;
        const int itemID = entry.itemID;
        // ボタン実行処理の登録
        button->RegisterOnClick([this, type, itemID]() {
            SelectButtonExecute(type, itemID);
        });
        // ボタンに navigation 更新処理を登録
        button->RegisterUpdateSelectButton([this, button]() {
            eventSystem.UpdateSelectButton(button);
        });
    }
}
/*
 *	@brief		ショップボタンの更新処理
 */
void MenuSelectShopItem::UpdateButtonState() {
    // すでにSMGを解放していたら、選択不可能状態にする
    UIButtonBase* button = nullptr;
    if (WeaponManager::GetInstance().IsSubmachineGun()) {
        button = FindButtonByName("SubmachineGun");
        if (button) button->SetIsEnable(false);
    }
    // すでに購入済みか判定
    if (IsBuyItem()) {
        button = FindButtonByName("Back");
        if(button) button->SetIsEnable(false);
    }
    else {
        button = FindButtonByName("Exit");
        if(button) button->SetIsEnable(false);
    }
}
/*
 *	@brief		名前でのボタン検索
 *	@param[in]	const std::string& buttonName
 *	@return		UIButtonBase*
 */
UIButtonBase* MenuSelectShopItem::FindButtonByName(const std::string& buttonName) {
    auto itr = buttonMap.find(buttonName);
    if (itr == buttonMap.end()) return nullptr;

    return itr->second;
}