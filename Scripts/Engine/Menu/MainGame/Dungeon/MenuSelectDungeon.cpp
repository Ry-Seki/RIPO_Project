/*
 *	@file	MenuSelectDungeon.cpp
 *	@author	Seki
 */

#include "MenuSelectDungeon.h"
#include "../../../Audio/AudioUtility.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../Load/Sprite/LoadSprite.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Input/InputUtility.h"
#include "../../../Save/SaveDataManager.h"
#include "../../../UI/Button/SinglePressButton.h"
#include "../../../UI/Text/Dynamic/DynamicText.h"
#include "../../../GameConst.h"
#include "../../../Engine.h"
#include "../../../Scene/MainGameScene.h"
#include "../../../../Data/UI/MenuInfo.h"
#include "../../MenuResourcesFactory.h"
#include "../../MenuManager.h"
#include "../../System/MenuConfirm.h"
#include "../MenuInGame.h"
#include "../../../Manager/FontManager.h"
#include "../../../../Data/Dungeon/DungeonData.h"

#include <DxLib.h>

 /*
  *	@brief	初期化処理
  */
void MenuSelectDungeon::Initialize(Engine& engine) {
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
		textList = std::move(result.textList);
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
			if (sprite->GetName() == "DungeonSprite") dungeonSprite = sprite.get();
		}
		eventSystem.LoadNavigation(navigation->GetData());
	});
}
/*
 *	@brief	メニューを開く
 */
void MenuSelectDungeon::Open() {
	MenuBase::Open();
	AudioUtility::ChangeBGM("selectDungeon");
	AudioUtility::PlayBGM();
	for (auto& sprite : spriteList) {
		sprite->Setup();
	}
	for (auto& button : buttonList) {
		button->Setup();
	}
	// TODO : ダンジョン3, 4を閉じる
	buttonList[2]->SetIsEnable(false);
	buttonList[3]->SetIsEnable(false);

	eventSystem.ApplySelection();
	currentIndex = eventSystem.GetCurrentIndex();
	CreateDungeonText();
	SetupText();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		// TODO : イベントごとに画像差し替え
		InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
	});
}
/*
 *	@brief	更新処理
 */
void MenuSelectDungeon::Update(Engine& engine, float unscaledDeltaTime) {
	auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

	// イベントシステムの更新
	eventSystem.Update(unscaledDeltaTime);
	currentIndex = eventSystem.GetCurrentIndex();
	dungeonSprite->SetFrameIndex(currentIndex);

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
void MenuSelectDungeon::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
	animTimer += unscaledDeltaTime;

	if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
	animTimer = 0;

	for (auto& sprite : spriteList) {
		if (!sprite || sprite.get() == dungeonSprite) continue;

		int frameCount = sprite->GetFrameCount();
		if (frameCount <= 1) continue;

		animFrame = (animFrame + 1) % frameCount;
		sprite->SetFrameIndex(animFrame);
	}
}
/*
 *	@brief	描画処理
 */
void MenuSelectDungeon::Render() {
	for (auto& sprite : spriteList) {
		if (!sprite->IsVisible() || sprite.get() == dungeonSprite) continue;
		sprite->Render();
	}
	dungeonSprite->Render();
	for (auto& button : buttonList) {
		if (!button->IsVisible()) continue;
		button->Render();
	}
	RenderDungeonInfo();
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectDungeon::Close(Engine& engine) {
	MenuBase::Close(engine);
	AudioUtility::StopBGM();
}
/*
 *	@brief	メニューを再開
 */
void MenuSelectDungeon::Resume() {
	MenuBase::Resume();
	AudioUtility::ChangeBGM(GameConst::_DUNGEONSELECT_BGM);
	AudioUtility::PlayBGM();
	for (auto& button : buttonList) {
		button->Setup();
	}
	// TODO : ダンジョン3, 4を閉じる
	buttonList[2]->SetIsEnable(false);
	buttonList[3]->SetIsEnable(false);
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int buttonIndex
 */
void MenuSelectDungeon::SelectButtonExecute(Engine& engine, int buttonIndex) {
	auto& menu = MenuManager::GetInstance();
	auto confirm = menu.GetMenu<MenuConfirm>();
	int dungeonID = 0;

	if (buttonIndex == 0) {
		dungeonID = 1;
		AudioUtility::PlaySE("DebugSE");
		confirm->SetCallback([this, &menu, dungeonID](GameEnum::ConfirmResult result) {
			if (result == GameEnum::ConfirmResult::Yes) {
				menu.CloseTopMenu();
				isInteractive = false;
				FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
				FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, dungeonID]() {
					menu.CloseAllMenu();
					if (Callback) Callback(dungeonID);
				});
			}else {
				menu.CloseTopMenu();
			}
		});
		menu.OpenMenu<MenuConfirm>();
	}
	else if (buttonIndex == 1) {
		dungeonID = 2;
		AudioUtility::PlaySE("DebugSE");
		confirm->SetCallback([this, &menu, dungeonID](GameEnum::ConfirmResult result) {
			if (result == GameEnum::ConfirmResult::Yes) {
				menu.CloseTopMenu();
				isInteractive = false;
				FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
				FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, dungeonID]() {
					menu.CloseAllMenu();
					if (Callback) Callback(dungeonID);
				});
			}else {
				menu.CloseTopMenu();
			}
		});
		menu.OpenMenu<MenuConfirm>();
	}
	else if (buttonIndex == 2) {
		dungeonID = 3;
		AudioUtility::PlaySE("DebugSE");
		confirm->SetCallback([this, &menu, dungeonID](GameEnum::ConfirmResult result) {
			if (result == GameEnum::ConfirmResult::Yes) {
				menu.CloseTopMenu();
				isInteractive = false;
				FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
				FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, dungeonID]() {
					menu.CloseAllMenu();
					if (Callback) Callback(dungeonID);
				});
			}else {
				menu.CloseTopMenu();
			}
		});
		menu.OpenMenu<MenuConfirm>();
	}
	else if (buttonIndex == 3) {
		dungeonID = 4;
		AudioUtility::PlaySE("DebugSE");
		confirm->SetCallback([this, &menu, dungeonID](GameEnum::ConfirmResult result) {
			if (result == GameEnum::ConfirmResult::Yes) {
				menu.CloseTopMenu();
				isInteractive = false;
				FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
				FadeManager::GetInstance().StartFade(fadeOut, [this, &menu, dungeonID]() {
					menu.CloseAllMenu();
					if (Callback) Callback(dungeonID);
				});
			}else {
				menu.CloseTopMenu();
			}
		});
		menu.OpenMenu<MenuConfirm>();
	}
}
/*
 *	@brief		テキストの生成
 */
void MenuSelectDungeon::CreateDungeonText() {
	DungeonInfoData& data = dungeonInfoList[currentIndex];
	int dataSize = dungeonInfoList.size();
	dungeonTextList.clear();
	dungeonTextList.resize(dataSize);
	auto& font = FontManager::GetInstance();
	int white = GetColor(255, 255, 255);

	for (int i = 0; i < dataSize; i++) {
		DungeonTextSet set;

		for (auto& text : textList) {
			TextInfo info = text->GetTextInfo();

			if (text->GetName() == "DungeonLevelText") {
				set.level = std::make_shared<DynamicText>(info);
				set.level->SetColor(white);
			}
			if (text->GetName() == "StrengthText") {
				set.strength = std::make_shared<DynamicText>(info);
				set.strength->SetColor(white);
			}
			if (text->GetName() == "TreasureCountText") {
				set.treasureCount = std::make_shared<DynamicText>(info);
				set.treasureCount->SetColor(white);
			}
			if (text->GetName() == "EventInfoText") {
				set.eventDay = std::make_shared<DynamicText>(info);
				set.eventDay->SetColor(white);
			}
		}
		dungeonTextList[i] = set;
	}
	std::string eventStart = std::to_string(data.eventStartDay);
	std::string eventEnd = std::to_string(data.eventEndDay);
	std::string eventDay = eventStart + " ～ " + eventEnd;

}
/*
 *	@brief		テキストの準備前処理
 */
void MenuSelectDungeon::SetupText() {
	for (int i = 0, max = dungeonTextList.size(); i < max; i++) {
		DungeonInfoData& data = dungeonInfoList[i];
		DungeonTextSet& dungeonInfo = dungeonTextList[i];

		dungeonInfo.level->SetText(std::to_string(data.levelOfDanger));
		dungeonInfo.treasureCount->SetText(std::to_string(data.treasureCount));
		dungeonInfo.strength->SetText(std::to_string(data.necessaryStrength));
		std::string eventDay;
		eventDay.reserve(32);
		eventDay += std::to_string(data.eventStartDay);
		eventDay += " ～ ";
		eventDay += std::to_string(data.eventEndDay);
		dungeonInfo.eventDay->SetText(data.isEventDay ? eventDay : "NONE");
	}
}
/*
 *	@brief		ダンジョン情報の描画
 */
void MenuSelectDungeon::RenderDungeonInfo() {
	if (currentIndex == -1) return;
	auto& font = FontManager::GetInstance();

	DungeonTextSet& dungeonInfo = dungeonTextList[currentIndex];

	dungeonInfo.level->Render();
	dungeonInfo.treasureCount->Render();
	dungeonInfo.strength->Render();
	dungeonInfo.eventDay->Render();
}
