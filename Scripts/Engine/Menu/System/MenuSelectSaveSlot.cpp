/*
 *	@file	MenuSelectSaveSlot.cpp
 *	@author	Seki
 */

#include "MenuSelectSaveSlot.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../UI/Button/UIButton.h"
#include "../../Input/InputUtility.h"
#include "../../Save/SaveDataManager.h"

/*
 *	@brief	初期化処理
 */
void MenuSelectSaveSlot::Initialize () {
    buttonList.resize(4);
    for (int i = 0; i < 4; i++) {
        // buttonList[i] = std::make_shared<UIButton>(Rect(0, 200 * i, 920, 180 + 200 * i));
        buttonList[i] = std::make_shared<UIButton>(Rect(200, 100 * i, 700, 80));
        buttonList[i]->SetName("SaveSlot");

        // ボタンの登録
        eventSystem.RegisterButton(buttonList[i].get());
    }
    // イベントシステムの初期化
    eventSystem.Initialize(0);

    auto& load = LoadManager::GetInstance();
    auto buttonHandle = load.LoadResource<LoadSprite>(_BUTTON_IMAGE_PATH);
    auto navigation   = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);

    load.SetOnComplete([this, navigation, buttonHandle]() {
        for (int i = 0, max = buttonList.size(); i < max; i++) {
            auto button = buttonList[i];
            if (!button) continue;

            button->RegisterButtonHandle(buttonHandle->GetHandle());
            button->SetOnClick([this, i]() {
                SelectButtonExecute(i);
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
	InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    for (auto& button : buttonList) {
        button->Setup();
    }
}
/*
 *	@brief	更新処理
 */
void MenuSelectSaveSlot::Update (Engine& engine, float deltaTime) {
    if (!IsInteractive()) return;

    eventSystem.Update(deltaTime);

    for (auto& button : buttonList) {
        if (button) button->Update(deltaTime);
    }
}
/*
 *	@brief	描画処理
 */
void MenuSelectSaveSlot::Render () {
    for (auto& button : buttonList) {
        button->Render();
    }
    DrawFormatString(100, 50, GetColor(255, 255, 255), "MenuSelectSaveSlot");
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectSaveSlot::Close () {
    MenuBase::Close();
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int slotIndex
 */
void MenuSelectSaveSlot::SelectButtonExecute(int slotIndex) {
    currentSlot = slotIndex;

    auto& save = SaveDataManager::GetInstance();

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
			break;
    }
}
