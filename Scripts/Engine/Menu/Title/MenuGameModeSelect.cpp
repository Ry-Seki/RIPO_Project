/*
 *	@file	MenuGameModeSelect.cpp
 *	@author	Seki
 */

#include "MenuGameModeSelect.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../../Audio/AudioUtility.h"
#include "../../../Engine/Engine.h"
#include "../../Scene/MainGameScene.h"
#include "../MenuManager.h"
#include "../Title/MenuSelectNewGame.h"
#include "../Title/MenuSelectLoadGame.h"
#include "../Title/MenuSystem.h"
#include "../System/MenuConfirm.h"
#include "../MenuResourcesFactory.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Input/InputUtility.h"
#include "../../Scene/TutorialScene.h"

#include <DxLib.h>

namespace {
	/*
	 *	@brief	ファイルパスの名前空間
	 */
	constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/Title/SelectGameMode/SelectGameModeMenuResources.json";
	constexpr const char* _NAVIGATION_PATH = "Data/UI/Title/SelectGameMode/SelectGameModeMenuNavigation.json";
	constexpr const char* _GAME_MODE_BUTTON_DATA_PATH = "Data/UI/Title/SelectGameMode/GameModeButtonData.json";

	// 別名定義
	using GameMode = GameEnum::GameMode;

	/*
	 *	@brief	ゲームモードのマップ
	 */
	const std::unordered_map<std::string, GameMode> gameModeMap = {
		{ "NewGame", GameMode::NewGame },
		{ "LoadGame", GameMode::LoadGame },
		{ "System", GameMode::System },
		{ "EndGame", GameMode::EndGame }
	};
	/*
	 *	@brief	ゲームモードボタン構造体
	 */
	struct GameModeButtonData {
		std::string name = "";
		GameMode mode = GameMode::Invalid;
	};
	/*
	 *	@brief		ゲームモードの識別
	 *	@param[in]	const std::string& typeKey
	 *	@return		GameMode
	 */
	GameMode StringToGameMode(const std::string& typeKey) {
		auto itr = gameModeMap.find(typeKey);

		if (itr != gameModeMap.end()) return itr->second;

		return GameMode::Invalid;
	}
	/*
	 *  @brief      JSON->ゲームモードボタン情報へ変換
	 *  @param[in]  const JSON& json
	 *	@return		std::vector<GameModeButtonData>
	 */
	std::vector<GameModeButtonData> ParseGameModeButtonData(const JSON& json) {
		std::vector<GameModeButtonData> result;

		auto array = json["GameModeButtons"];

		for (auto& node : array) {
			GameModeButtonData entry;

			entry.name = node["ButtonName"].get<std::string>();

			std::string typeString = node["GameMode"].get<std::string>();
			entry.mode = StringToGameMode(typeString);

			result.push_back(entry);
		}
		return result;;
	}
}
/*
 *	@brief	初期化処理
 */
void MenuGameModeSelect::Initialize(Engine& engine) {
	auto& load = LoadManager::GetInstance();
	auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
	auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);
	auto buttonData = load.LoadResource<LoadJSON>(_GAME_MODE_BUTTON_DATA_PATH);

	load.SetOnComplete([this, &engine, menuJSON, navigation, buttonData]() {
		// メニューUI生成
		MenuInfo result = MenuResourcesFactory::Create(menuJSON->GetData());
		// メニューUIの所有権移動
		buttonList = std::move(result.buttonList);
		spriteList = std::move(result.spriteList);
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
		SetupGameModeButtons(buttonData->GetData(), engine);
		// イベントシステムのnavigationの設定
		eventSystem.LoadNavigation(navigation->GetData());
	});
}
/*
 *	@brief	メニューを開く
 */
void MenuGameModeSelect::Open() {
	MenuBase::Open();
	for (auto& sprite : spriteList) {
		sprite->Setup();
	}
	for (auto& button : buttonList) {
		button->Setup();
	}
	eventSystem.ApplySelection();
	InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
}
/*
 *	@brief	更新処理
 */
void MenuGameModeSelect::Update(Engine& engine, float unscaledDeltaTime) {
	auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

	if (input.buttonDown[static_cast<int>(GameEnum::MenuAction::Cancel)]) {
		CheckEndGame(engine);
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

	if (!inputHandle && input.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]) {
		inputHandle = true;
		button->OnPressDown();
	}
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuGameModeSelect::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
	animTimer += unscaledDeltaTime;

	if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
	animTimer = 0;

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
void MenuGameModeSelect::Render() {
	for (auto& sprite : spriteList) {
		if (!sprite->IsVisible()) continue;
		sprite->Render();
	}
	for (auto& button : buttonList) {
		if (!button->IsVisible()) continue;
		button->Render();
	}
}
/*
 *	@brief	メニューを閉じる
 */
void MenuGameModeSelect::Close(Engine& engine) {
	MenuBase::Close(engine);
}
/*
 *	@brief	メニューを再開
 */
void MenuGameModeSelect::Resume() {
	MenuBase::Resume();
	for (auto& button : buttonList) {
		button->Setup();
	}
	eventSystem.ApplySelection();
}
/*
 *	@brief		ボタンの押された時の処理
 *	@param[in]	GameEnum::GameMode mode
 */
void MenuGameModeSelect::SelectButtonExecute(GameEnum::GameMode mode, Engine& engine) {
	auto& menu = MenuManager::GetInstance();
	auto confirm = menu.GetMenu<MenuConfirm>();
	AudioUtility::PlaySE("DebugSE");

	switch (mode) {
		case GameEnum::GameMode::NewGame:
			confirm->SetCallback([this, confirm, &menu, &engine](GameEnum::ConfirmResult result) {
				AudioUtility::PlaySE("DebugSE");
				menu.CloseTopMenu();
				if (result != GameEnum::ConfirmResult::Yes) return;
				isInteractive = false;
				menu.CloseAllMenu();
				engine.SetNextScene(std::make_shared<TutorialScene>());
			});
			menu.OpenMenu<MenuConfirm>();
			break;
		case GameEnum::GameMode::LoadGame:{
			isInteractive = false;
			FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 0.5f, FadeDirection::Out, FadeMode::Stop);
			FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
				menu.OpenMenu<MenuSelectLoadGame>();
			});
		}
			break;
		case GameEnum::GameMode::System:
			menu.OpenMenu<MenuSystem>();
			break;
		case GameEnum::GameMode::EndGame:
			CheckEndGame(engine);
			break;
		default:
			break;
	}
}
/*
 *	@brief		ゲームモードボタンの準備前処理
 *	@param[in]	const JSON& json
 */
void MenuGameModeSelect::SetupGameModeButtons(const JSON& json, Engine& engine) {
	auto gameModeData = ParseGameModeButtonData(json);
	for (const auto& entry : gameModeData) {
		UIButtonBase* button = FindButtonByName(entry.name);
		if (!button) continue;
		const auto mode = entry.mode;
		// ボタン実行処理の登録
		button->RegisterOnClick([this, &engine, mode]() {
			SelectButtonExecute(mode, engine);
		});
		// ボタンに navigation 更新処理を登録
		button->RegisterUpdateSelectButton([this, button]() {
			eventSystem.UpdateSelectButton(button);
		});
	}
}
/*
 *	@brief		ゲーム終了処理
 */
void MenuGameModeSelect::CheckEndGame(Engine& engine) {
	auto& menu = MenuManager::GetInstance();
	auto confirm = menu.GetMenu<MenuConfirm>();

	confirm->SetCallback([this, &menu, &engine](GameEnum::ConfirmResult result) {
		AudioUtility::PlaySE("DebugSE");
		menu.CloseTopMenu();	// このメニュー
		if (result != GameEnum::ConfirmResult::Yes) return;

		isInteractive = false;
		menu.CloseAllMenu();
		engine.SetIsGameEnd(true);
	});
	menu.OpenMenu<MenuConfirm>();
}
/*
 *	@brief		名前でのボタン検索
 *	@param[in]	const std::string& buttonName
 *	@return		UIButtonBase*
 */
UIButtonBase* MenuGameModeSelect::FindButtonByName(const std::string& buttonName) {
	auto itr = buttonMap.find(buttonName);
	if (itr == buttonMap.end()) return nullptr;

	return itr->second;
}