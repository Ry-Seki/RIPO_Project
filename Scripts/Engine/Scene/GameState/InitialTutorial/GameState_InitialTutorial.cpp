/*
 *	@file	GameState_InitialTutorial.cpp
 *	@author	Seki
 */

#include "GameState_InitialTutorial.h"
#include "../GameStateMachine.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/Dungeon/DungeonDataLoader.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Audio/AudioUtility.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../JSON.h"
#include "../../../../Data/Dungeon/DungeonStageData.h"
#include "../../../../Data/Dungeon/DungeonFloorData.h"

/*
 *	@brief	初期化処理
 */
void GameState_InitialTutorial::Initialize(Engine& engine) {
	LoadManager& load = LoadManager::GetInstance();
	dungeonDataLoader = load.LoadResource<DungeonDataLoader>(_DUNGEON_LIST_PATH);
	load.SetOnComplete([this]() { dungeonDataList = dungeonDataLoader->dungeonList; });
}
/*
 *	@brief	準備前処理
 */
void GameState_InitialTutorial::Setup() {
	auto& context = owner->GetActionContext();
	context.actionType = GameEnum::ActionType::Dungeon;
	StartDungeonDataLoad();
}
/*
 *	@brief	更新処理
 */
void GameState_InitialTutorial::Update(float deltaTime) {
	
}
/*
 *	@brief	描画処理
 */
void GameState_InitialTutorial::Render() {
}
/*
 *	@brief	片付け処理
 */
void GameState_InitialTutorial::Teardown() {
}
/*
 *	@brief		ダンジョンデータのロード開始
 */
void GameState_InitialTutorial::StartDungeonDataLoad() {
	LoadManager& load = LoadManager::GetInstance();
	DungeonData dungeonData = dungeonDataList[0];
	// ファイルパスの取得
	std::string filePath = dungeonData.dungeonPath;
	if (filePath.empty()) return;
	// ダンジョンステージデータの読み込み
	std::vector<std::shared_ptr<LoadJSON>> jsonList;
	jsonList.push_back(load.LoadResource<LoadJSON>(filePath));
	jsonList.push_back(load.LoadResource<LoadJSON>(_DUNGEON_FLOOR_PATH));
	// コールバック登録
	load.SetOnComplete([this, jsonList]() {SetDungeonData(jsonList); });
}
/*
 *	@brief		読み込んだダンジョンデータの設定
 *  @param[in]	const std::vector<std::shared_ptr<LoadJSON>>& setDataList
 */
void GameState_InitialTutorial::SetDungeonData(const std::vector<std::shared_ptr<LoadJSON>>& setDataList) {
	// JSONデータの取得
	JSON dungeonData = setDataList[0]->GetData();
	JSON dungeonfloorData = setDataList[1]->GetData();
	if (dungeonData.empty() || dungeonfloorData.empty()) return;

	auto& context = owner->GetActionContext();
	// それぞれのデータを初期化
	context.dungeonID = 0;
	context.isCurrentEvent = dungeonDataList[0].isEventDay;
	context.dungeonStageData.LoadFromJSON(dungeonData);
	context.dungeonFloorData.LoadFromJSON(dungeonfloorData, 0);
	// ステートの切り替え
	owner->ChageState(GameEnum::GameState::InAction);
}
