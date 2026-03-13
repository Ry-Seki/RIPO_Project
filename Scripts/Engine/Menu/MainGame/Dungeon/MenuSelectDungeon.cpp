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

namespace {
	/*
	 *	@brief	ファイルパスの名前空間
	 */
	constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Dungeon/SelectDungeon/SelectDungeonMenuResources.json";
	constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/Dungeon/SelectDungeon/SelectDungeonMenuNavigation.json";
	constexpr const char* _DUNGEON_BUTTON_DATA_PATH = "Data/UI/MainGame/Dungeon/SelectDungeon/DungeonButton.json";

	// 別名定義
	using DungeonType = GameEnum::DungeonType;

	/*
	 *	@brief	ダンジョンの種類マップ
	 */
	const std::unordered_map<std::string, GameEnum::DungeonType> dungeonTypeMap = {
		{ "Dungeon1", DungeonType::Dungeon1 },
		{ "Dungeon2", DungeonType::Dungeon2 },
		{ "Dungeon3", DungeonType::Dungeon3 },
		{ "Dungeon4", DungeonType::Dungeon4 },
		{ "Back", DungeonType::Invalid }
	};
	/*
	 *	@brief	ダンジョンボタン構造体
	 */
	struct DungeonButtonData {
		std::string name = "";
		DungeonType type = DungeonType::Invalid;
	};
	/*
	 *	@brief		ダンジョンの種類識別
	 *	@param[in]	const std::string& typeKey
	 *	@return		DungeonType
	 */
	DungeonType StringToDungeonType(const std::string& typeKey) {
		auto itr = dungeonTypeMap.find(typeKey);

		if (itr != dungeonTypeMap.end()) return itr->second;

		return DungeonType::Invalid;
	}
	/*
	 *  @brief      JSON->ダンジョンボタン情報へ変換
	 *  @param[in]  const JSON& json
	 *	@return		std::vector<DungeonButtonData>
	 */
	std::vector<DungeonButtonData> ParseDungeonButtonData(const JSON& json) {
		std::vector<DungeonButtonData> result;

		auto array = json["DungeonButtons"];

		for (auto& node : array) {
			DungeonButtonData entry;

			entry.name = node["ButtonName"].get<std::string>();

			std::string typeString = node["DungeonType"].get<std::string>();
			entry.type = StringToDungeonType(typeString);

			result.push_back(entry);
		}
		return result;
	}
}
 /*
  *	@brief	初期化処理
  */
void MenuSelectDungeon::Initialize(Engine& engine) {
	auto& load = LoadManager::GetInstance();

	auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
	auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
	auto buttonData = load.LoadResource<LoadJSON>(_DUNGEON_BUTTON_DATA_PATH);

	load.SetOnComplete([this, &engine, menuJSON, navigation, buttonData]() {
		// メニューUI生成
		MenuInfo result = MenuResourcesFactory::Create(menuJSON->GetData());
		// メニューUIの所有権移動
		spriteList = std::move(result.spriteList);
		textList = std::move(result.textList);
		buttonList = std::move(result.buttonList);
		// ボタンの登録
		for (auto& entry : buttonList) {
			if (!entry) continue;
			UIButtonBase* button = entry.get();
			// ボタンの登録
			eventSystem.RegisterButton(button);
			// マップに登録
			buttonMap[button->GetName()] = button;
		}
		// ダンジョン画像の取得
		for (auto& sprite : spriteList) {
			if (sprite->GetName() == "DungeonSprite") dungeonSprite = sprite.get();
		}
		// イベントシステムの初期化
		eventSystem.Initialize(0);
		// ボタンの準備前処理
		SetupDungeonButtons(buttonData->GetData());
		// イベントシステムのnavigationの設定
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
	eventSystem.ApplySelection();
	// 現在のボタンの要素数の取得
	currentIndex = eventSystem.GetCurrentIndex();
	prevIndex = currentIndex;
	// ダンジョンテキストの生成
	CreateDungeonInfoData();
	// ダンジョン情報の準備前処理
	SetupDungeonInfo();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
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

	TextBase* level = nullptr;
	TextBase* strength = nullptr;
	TextBase* treasure = nullptr;
	TextBase* eventInfo = nullptr;

	for (auto& text : textList) {
		if (!text) continue;
		// 色の設定
		text->SetColor(white);
		// テキストの設定
		if (text->GetName() == "DungeonLevelText") level = text.get();
		else if (text->GetName() == "StrengthText") strength = text.get();
		else if (text->GetName() == "TreasureCountText") treasure = text.get();
		else if (text->GetName() == "EventInfoText") eventInfo = text.get();
	}
	// 情報データの数dungeonMenuListを追加する
	for (auto& data : dungeonInfoList) {
		DungeonMenuEntry entry;

		entry.info = data;
		entry.level = level;
		entry.strength = strength;
		entry.treasureCount = treasure;
		entry.eventInfo = eventInfo;

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

	SortDungeonMenuEntry(entry);
}
/*
 *	@brief		ダンジョン情報の描画
 */
void MenuSelectDungeon::RenderDungeonInfo() {
	if (currentIndex < 0 || currentIndex >= dungeonMenuList.size()) return;

	// ダンジョンの種類の取得
	auto dungeonType = static_cast<GameEnum::DungeonType>(currentIndex);
	if (dungeonType == GameEnum::DungeonType::Invalid) return;

	dungeonSprite->Render();

	auto& entry = dungeonMenuList[currentIndex];

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
	entry.eventInfo->SetText(eventStr);
}
/*
 *	@brief		ダンジョンボタンの準備前処理
 *	@param[in]	const JSON& json
 */
void MenuSelectDungeon::SetupDungeonButtons(const JSON& json) {
	auto dungeonData = ParseDungeonButtonData(json);

	for (const auto& entry : dungeonData) {
		UIButtonBase* button = FindButtonByName(entry.name);
		if (!button) continue;
		const auto type = entry.type;
		int dungeonID = static_cast<int>(type);
		// ボタン実行処理の登録
		button->RegisterOnClick([this, dungeonID]() {
			SelectButtonExecute(dungeonID);
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
UIButtonBase* MenuSelectDungeon::FindButtonByName(const std::string& buttonName) {
	auto itr = buttonMap.find(buttonName);
	if (itr == buttonMap.end()) return nullptr;

	return itr->second;
}
