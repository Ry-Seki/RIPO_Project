/*
 *	@file	MenuTitle.cpp
 *	@author	Seki
 */

#include "MenuTitle.h"
#include "../MenuManager.h"
#include "../MenuResourcesFactory.h"
#include "MenuGameModeSelect.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../../Audio/AudioUtility.h"
#include "../../Input/InputUtility.h"
/*
 *	@brief	初期化処理
 */
void MenuTitle::Initialize(Engine& engine) {
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
 *	@brief	メニューを開く処理
 */
void MenuTitle::Open() {
	MenuBase::Open();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
		InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
	});
}
/*
 *	@brief	更新処理
 */
void MenuTitle::Update(Engine& engine, float unscaledDeltaTime) {
	if (!isStart) return;

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
void MenuTitle::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
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
void MenuTitle::Render() {
	for (auto& sprite : spriteList ) {
		sprite->Render();
	}
	for (auto& button : buttonList) {
		button->Render();
	}
	DrawFormatString(300, 400, GetColor(255, 255, 255), "Play->SpaceKey");
}
/*
 *	@brief	メニューを閉じる処理
 */
void MenuTitle::Close(Engine& engine) {
	MenuBase::Close(engine);
}
/*
 *	@brief	メニューを再開
 */
void MenuTitle::Resume() {
	MenuBase::Resume();
}
/*
 *	@brief		ロード済みデータのセット
 *  @prarm[in]	int setHandle
 */
void MenuTitle::SetupData(int setHandle) {
}

void MenuTitle::SelectButtonExecute(Engine& engine, int buttonIndex) {
	if (buttonIndex == 0) {
		AudioUtility::PlaySE("DebugSE");
		inputHandle = true;
		FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::InkSpread, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fadeOut, [this]() {
			MenuManager::GetInstance().OpenMenu<MenuGameModeSelect>();
		});
	}
}
