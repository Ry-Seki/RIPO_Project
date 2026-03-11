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
#include "../../../System/World/WorldProgressManager.h"

#include <DxLib.h>

/*
 *	@brief	ファイルパスの名前空間
 */
namespace {
	constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Dungeon/SelectDungeon/SelectDungeonMenuResources.json";
	constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame//Dungeon/SelectDungeon/SelectDungeonMenuNavigation.json";
}
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
		const int back = static_cast<int>(GameEnum::MainDungeonType::Invalid);
		const int dungeonMin = static_cast<int>(GameEnum::MainDungeonType::Dungeon1);
		const int dungeonMax = static_cast<int>(GameEnum::MainDungeonType::Max);
		for (int i = 0, max = buttonList.size(); i < max; i++) {
			UIButtonBase* button = buttonList[i].get();
			if (!button) continue;
			// ダンジョンIDの取得
			int dungeonID = (i < dungeonMax - 1) ? dungeonMin + i : back;
			// ダンジョンボタンリストに登録
			dungeonButtonList.push_back({dungeonID, button});
			// ボタンの実行処理を登録
			button->RegisterOnClick([this, dungeonID]() {
				SelectButtonExecute(dungeonID);
			});
			// ボタンに navigation 更新処理を登録
			button->RegisterUpdateSelectButton([this, button]() {
				eventSystem.UpdateSelectButton(button);
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
	//buttonList[2]->SetIsEnable(false);
	//buttonList[3]->SetIsEnable(false);

	eventSystem.ApplySelection();
	currentIndex = eventSystem.GetCurrentIndex();
	prevIndex = currentIndex;
	CreateDungeonInfoData();
	SetupDungeonInfo();
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
	// 選択されているボタンが変更されたら文字の再設定
	if (prevIndex != currentIndex) {
		prevIndex = currentIndex;
		SetupDungeonInfo();
	}


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
	//buttonList[2]->SetIsEnable(false);
	//buttonList[3]->SetIsEnable(false);
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	int dungeonID
 */
void MenuSelectDungeon::SelectButtonExecute(int dungeonID) {
	AudioUtility::PlaySE("DebugSE");
	// 戻る
	if (dungeonID == -1) {
		StartFadeEndCallback(dungeonID);
		return;
	}
	// 確認メニューを開く
	OpenConfirmMenu(dungeonID);
}
/*
 *	@brief		ダンジョン情報データの生成
 */
void MenuSelectDungeon::CreateDungeonInfoData() {
	dungeonMenuList.clear();

	int white = GetColor(255, 255, 255);

	for (auto& data : dungeonInfoList) {
		MenuSelectDungeon::DungeonMenuEntry entry;
		entry.info = data;

		for (auto& text : textList) {
			if (text->GetName() == "DungeonLevelText") {
				entry.level = text.get();
				entry.level->SetColor(white);
			}
			else if (text->GetName() == "StrengthText") {
				entry.strength = text.get();
				entry.strength->SetColor(white);
			}
			else if (text->GetName() == "TreasureCountText") {
				entry.treasureCount = text.get();
				entry.treasureCount->SetColor(white);
			}
			else if (text->GetName() == "EventInfoText") {
				entry.eventInfo = text.get();
				entry.eventInfo->SetColor(white);
			}
		}
		dungeonMenuList.push_back(entry);
	}
}
/*
 *	@brief		テキストの準備前処理
 */
void MenuSelectDungeon::SetupDungeonInfo() {
	if (currentIndex < 0 || currentIndex >= dungeonMenuList.size()) return;
	// ダンジョン画像の設定
	dungeonSprite->SetFrameIndex(currentIndex);
	// ダンジョン情報テキストの設定
	auto& entry = dungeonMenuList[currentIndex];
	entry.level->SetText(std::to_string(entry.info.levelOfDanger));
	entry.strength->SetText(
		std::to_string(entry.info.minStrength)
		+ " ～ "
		+ std::to_string(entry.info.maxStrength));
	entry.treasureCount->SetText(
		std::to_string(entry.info.treasureCount)
		+ " / "
		+ std::to_string(entry.info.maxTreasureCount));

	std::string eventStr = "";
	if (entry.info.isEventClear) {
		eventStr = "Complete!";
	}else {
		if (entry.info.isEventDay) {
			eventStr = std::to_string(entry.info.eventStartDay)
				+ " ～ "
				+ std::to_string(entry.info.eventEndDay);
		}
		else {
			eventStr = "NONE";
		}
	}
	entry.eventInfo->SetText(eventStr);
}
/*
 *	@brief		ダンジョン情報の描画
 */
void MenuSelectDungeon::RenderDungeonInfo() {
	if (currentIndex < 0 || currentIndex >= dungeonButtonList.size()) return;

	int dungeonID = dungeonButtonList[currentIndex].dungeonID;

	// 戻るボタンなら描画しない
	if (dungeonID <= 0) return;

	dungeonSprite->Render();

	DungeonMenuEntry& entry = dungeonMenuList[currentIndex];
	if (entry.level) entry.level->Render();
	if (entry.strength) entry.strength->Render();
	if (entry.treasureCount) entry.treasureCount->Render();
	if (entry.eventInfo) entry.eventInfo->Render();
}

/*
 *	@brief		フェード後->コールバックの実行処理
 *	@param[in]	int dungeonID
 */
void MenuSelectDungeon::StartFadeEndCallback(int dungeonID) {
	auto& menu = MenuManager::GetInstance();

	isInteractive = false;
	FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeOut,[this, &menu, dungeonID]() {
		menu.CloseTopMenu();	// このメニュー
		if (Callback) Callback(dungeonID);
	});
}
/*
 *	@brief		確認メニューを開く
 *	@param[in]	int dungeonID
 */
void MenuSelectDungeon::OpenConfirmMenu(int dungeonID) {
	auto& menu = MenuManager::GetInstance();
	auto confirm = menu.GetMenu<MenuConfirm>();

	confirm->SetCallback([this, &menu, dungeonID](GameEnum::ConfirmResult result) {
		menu.CloseTopMenu();	// 確認メニュー
		if (result != GameEnum::ConfirmResult::Yes) return;
		// フェード開始
		StartFadeEndCallback(dungeonID);
	});
	menu.OpenMenu<MenuConfirm>();
}
/*
 *	@brief		イベント情報を整理
 *	@param[in]	DungeonMenuEntry& entry
 */
void MenuSelectDungeon::SortDungeonMenuEntry(DungeonMenuEntry& entry) {
	std::string eventStr = "";
	// すでに獲得していた場合、クリアとする
	if (entry.info.isEventClear) {
		eventStr = "Complete!";
	}
	else {
		// もしイベント中なら、イベント日を入れる
		if (entry.info.isEventDay) {

			eventStr = std::to_string(entry.info.eventStartDay)
				+ " ～ "
				+ std::to_string(entry.info.eventEndDay);
		}else {
			eventStr = "NONE";
		}
	}
	entry.dungeonEvent.eventText->SetText(eventStr);
}