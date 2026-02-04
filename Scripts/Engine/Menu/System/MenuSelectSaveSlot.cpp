/*
 *	@file	MenuSelectSaveSlot.cpp
 *	@author	Seki
 */

#include "MenuSelectSaveSlot.h"
#include "../MenuManager.h"
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

/*
 *	@brief	初期化処理
 */
void MenuSelectSaveSlot::Initialize (Engine& engine) {
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
void MenuSelectSaveSlot::Open () {
    MenuBase::Open();
	currentSlot = -1;
    // ボタンの準備処理
    for (auto& button : buttonList) {
        button->Setup();
    }
    // ロード時、セーブスロットが未使用だった場合選択不可にする
    if (saveMode == GameEnum::SaveSlotMenuMode::Load) {
        // セーブスロットの使用状態の取得
        auto& save = SaveDataManager::GetInstance();
        std::vector<bool> isUsedList = save.GetAllSlotIsUsed();
        for (int i = 0, max = buttonList.size(); i < max; i++) {
            auto button = buttonList[i];
            if (!button) continue;

            if (button == buttonList.back()) continue;

            if (!isUsedList[i]) button->SetIsEnable(false);
        }
    }
    eventSystem.ApplySelection();
	InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
}
/*
 *	@brief	更新処理
 */
void MenuSelectSaveSlot::Update (Engine& engine, float unscaledDeltaTime) {
    auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

    if (input.buttonDown[static_cast<int>(GameEnum::MenuAction::Cancel)]) {
        MenuManager::GetInstance().CloseTopMenu();
        return;
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
 *	@brief	描画処理
 */
void MenuSelectSaveSlot::Render () {
    for (auto& button : buttonList) {
        button->Render();
    }
    DrawFormatString(50, 125, GetColor(255, 255, 255), "MenuSelectSaveSlot");
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectSaveSlot::Close (Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int slotIndex
 */
void MenuSelectSaveSlot::SelectButtonExecute(Engine& engine, int slotIndex) {
    currentSlot = slotIndex;

    auto& save = SaveDataManager::GetInstance();
    auto& menu = MenuManager::GetInstance();

    // もし一番下のボタンの場合、それは戻るボタン
    if (currentSlot > GameConst::SELECT_SAVE_SLOT_MAX) {
        menu.CloseTopMenu();
        return;
    }

    switch (saveMode) {
        case GameEnum::SaveSlotMenuMode::Save:
			// TODO : 上書きをするか確認するメニュー表示
			save.SelectSlot(currentSlot);
			save.SaveCurrentSlot();
			// TODO : 確定メニューを表示
            break;

        case GameEnum::SaveSlotMenuMode::Load:
			// TODO : ロードをするか確認するメニュー表示
			save.SelectSlot(currentSlot);
			save.LoadCurrentSlot();
			// TODO : 読み込むー＞シーンの読み込み
            engine.SetNextScene(std::make_shared<MainGameScene>());
            MenuManager::GetInstance().CloseAllMenu();
			break;
    }
}
