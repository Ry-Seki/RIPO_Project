/*
 *	@file	MenuSelectSaveSlot.cpp
 *	@author	Seki
 */

#include "MenuSelectSaveSlot.h"
#include "MenuConfirm.h"
#include "../MenuManager.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Input/InputUtility.h"
#include "../../Save/SaveDataManager.h"
#include "../../Engine.h"
#include "../../Scene/MainGameScene.h"
#include "../MenuResourcesFactory.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Manager/FontManager.h"
#include "../../Fade/FadeManager.h"
#include "../../Fade/FadeFactory.h"

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
        for (auto& sprite : spriteList) {
            if (sprite->GetName() == "SaveLoad") saveLoadSprite = sprite.get();
        }
        eventSystem.LoadNavigation(navigation->GetData());
    });
    gameDataList.reserve(4);
}
/*
 *	@brief	メニューを開く
 */
void MenuSelectSaveSlot::Open () {
    MenuBase::Open();
    // セーブスロットの使用状態の取得
    auto& save = SaveDataManager::GetInstance();
	currentSlot = -1;
    gameDataList.clear();

    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    // ボタンの準備処理
    for (auto& button : buttonList) {
        button->Setup();
    }
    saveLoadSprite->SetFrameIndex(static_cast<int>(saveMode));
    // セーブデータの取得
    auto allData = save.GetAllSlotData();
    for (const auto& data : allData) {
        gameDataList.push_back(data.game);
    }
    // ロード時、セーブスロットが未使用だった場合選択不可にする
    if (saveMode == GameEnum::SaveSlotMenuMode::Load) {
        isUsedList = save.GetAllSlotIsUsed();
        for (int i = 0, max = buttonList.size() - 1; i < max; i++) {
            auto button = buttonList[i];
            if (!button) continue;

            if (button == buttonList.back()) continue;

            bool isUsed = isUsedList[i];
            button->SetIsEnable(isUsed);
        }
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
void MenuSelectSaveSlot::Update (Engine& engine, float unscaledDeltaTime) {
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
void MenuSelectSaveSlot::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
    animTimer += unscaledDeltaTime;

    if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
    animTimer -= GameConst::UI_ANIM_INTERVAL;

    for (auto& sprite : spriteList) {
        if (!sprite || sprite.get() == saveLoadSprite) continue;

        int frameCount = sprite->GetFrameCount();
        if (frameCount <= 1) continue;

        animFrame = (animFrame + 1) % frameCount;
        sprite->SetFrameIndex(animFrame);
    }
}
/*
 *	@brief	描画処理
 */
void MenuSelectSaveSlot::Render () {
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible() || sprite.get() == saveLoadSprite) continue;
        sprite->Render();
    }
    saveLoadSprite->Render();
    for (auto& button : buttonList) {
        if (!button->IsVisible()) continue;
        button->Render();
    }
    RenderSlotInfo();
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectSaveSlot::Close (Engine& engine) {
    MenuBase::Close(engine);
}
void MenuSelectSaveSlot::Resume() {
    MenuBase::Resume();
    for (auto& button : buttonList) {
        button->Setup();
    }
    auto& save = SaveDataManager::GetInstance();
    gameDataList.clear();
    // ロード時、セーブスロットが未使用だった場合選択不可にする
    if (saveMode == GameEnum::SaveSlotMenuMode::Save) {
        auto allData = save.GetAllSlotData();
        for (const auto& data : allData) {
            gameDataList.push_back(data.game);
        }
        isUsedList = save.GetAllSlotIsUsed();
        for (int i = 0, max = buttonList.size() - 1; i < max; i++) {
            auto button = buttonList[i];
            if (!button) continue;

            if (button == buttonList.back()) continue;

            bool isUsed = isUsedList[i];
            button->SetIsEnable(isUsed);
        }
    }
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int slotIndex
 */
void MenuSelectSaveSlot::SelectButtonExecute(Engine& engine, int slotIndex) {
    currentSlot = slotIndex;

    auto& save = SaveDataManager::GetInstance();
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();

    // もし一番下のボタンの場合、それは戻るボタン
    if (currentSlot > GameConst::SELECT_SAVE_SLOT_MAX) {
        menu.CloseTopMenu();
        return;
    }

    switch (saveMode) {
        case GameEnum::SaveSlotMenuMode::Save:
            confirm->SetCallback([this, &save, &menu](GameEnum::ConfirmResult result) {
                if (result == GameEnum::ConfirmResult::Yes) {
                    save.SelectSlot(currentSlot);
                    save.SaveCurrentSlot();
                    menu.CloseTopMenu();
                } else {
                    menu.CloseTopMenu();
                }
            });
            menu.OpenMenu<MenuConfirm>();

			// TODO : 確定メニューを表示
            break;

        case GameEnum::SaveSlotMenuMode::Load:
            confirm->SetCallback([this, &engine, &save, &menu](GameEnum::ConfirmResult result) {
                if (result == GameEnum::ConfirmResult::Yes) {
                    save.SelectSlot(currentSlot);
                    save.LoadCurrentSlot();
                    engine.SetNextScene(std::make_shared<MainGameScene>());
                    menu.CloseAllMenu();
                } else {
                    menu.CloseTopMenu();
                }
            });
            menu.OpenMenu<MenuConfirm>();
			break;
    }
}
/*
 *	@brief		セーブスロット情報の描画
 */
void MenuSelectSaveSlot::RenderSlotInfo() {
    auto& font = FontManager::GetInstance();
    int white = GetColor(255, 255, 255);
    for (int i = 0, max = gameDataList.size(); i < max; i++) {
        if (!isUsedList[i]) continue;
        std::string elapsedDayStr = std::to_string(gameDataList[i].elapsedDay);
        std::string halfDayStr;
        if (gameDataList[i].isHalfDay) {
            halfDayStr = "[PM]";
        } else {
            halfDayStr = "[AM]";
        }
        std::string playTimeStr = std::to_string(gameDataList[i].playTime);
        std::string moneyStr = std::to_string(gameDataList[i].currentMoney);
        std::string treasureStr = std::to_string(gameDataList[i].totalTreasureCount);

        font.Draw("ElapsedDayFont", 1295, 273 + 205 * i, elapsedDayStr, white);
        font.Draw("ElapsedDayFont", 1370, 273 + 205 * i, halfDayStr, white);
        font.Draw("SaveSlotFont", 1671, 232 + 205 * i, playTimeStr, white);
        font.Draw("SaveSlotFont", 1671, 269 + 205 * i, moneyStr, white);
        font.Draw("SaveSlotFont", 1671, 302 + 205 * i, treasureStr, white);
    }
}
