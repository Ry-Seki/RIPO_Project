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

/*
 *	@brief	初期化処理
 */
void MenuSelectSaveSlot::Initialize () {
	// ボタンの生成
	for (int i = 0; i < 4; i++) {
		buttonList[i] = std::make_shared<UIButton>(Rect(0, 0 + 200 * i, 920, 180 + 200 * i));
		buttonList[i]->SetName("SaveSlot");
	}
	auto& load = LoadManager::GetInstance ();
	auto buttonHandle = load.LoadResource<LoadJSON>(_BUTTON_IMAGE_PATH);
	auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
	load.SetOnComplete ([this, &navigation, &buttonHandle]() {
		for (int i = 0, max = buttonList.size(); i < max; i++) {
			buttonList[i]->RegisterButtonHandle(buttonHandle->GetData());
		}
		eventSystem.LoadNavigation (navigation->GetData ());
	});

}
/*
 *	@brief	メニューを開く
 */
void MenuSelectSaveSlot::Open () {
	InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
}
/*
 *	@brief	更新処理
 */
void MenuSelectSaveSlot::Update (Engine& engine, float deltaTime) {
}
/*
 *	@brief	描画処理
 */
void MenuSelectSaveSlot::Render () {
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectSaveSlot::Close () {
}
