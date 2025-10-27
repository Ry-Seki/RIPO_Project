/*
 *	@file	SelectionDungeon.cpp
 *	@author	Seki
 */

#include "SelectionDungeon.h"
#include "../../../Load/LoadManager.h"
#include "../../DayAction/ActionManager.h"
#include "../../Selection/SelectionManager.h"


void SelectionDungeon::Initialize(Engine& engine) {
	isComplete = false;
	dungeonDataLoader = std::make_shared<DungeonDataLoader>("Data/Dungeon/DungeonList.csv");
	LoadManager::GetInstance().AddLoader(dungeonDataLoader);
	LoadManager::GetInstance().SetOnComplete( [this, &engine]() { Setup(engine); } );
}

void SelectionDungeon::Setup(Engine& engine) {
	dungeonDataList = dungeonDataLoader->dungeonList;
	LoadManager::GetInstance().Clear();
}

void SelectionDungeon::Update(Engine& engine, float deltaTime) {
	if (dungeonDataList.empty() || isComplete) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_0)) { 
		inputHandle = true;
		isComplete = true;
		StartStageDataLoad(engine, 0);
	}

	if (CheckHitKey(KEY_INPUT_0) == 0) inputHandle = false;
}

void SelectionDungeon::Render() {
	DrawFormatString(50, 50, GetColor(0, 0, 0), "0: TutorialDungeon");
}

void SelectionDungeon::StartStageDataLoad(Engine& engine, int dungeonID) {
	DungeonData dungeonData = dungeonDataList[dungeonID];
	// ファイルパスの取得
	std::string filePath = dungeonData.dungeonPath;
	if (filePath.empty()) return;
	// ダンジョンステージデータの読み込み
	auto dungeonStageLoader = std::make_shared<LoadJSON>(filePath);
	LoadManager::GetInstance().AddLoader(dungeonStageLoader);
	// コールバック登録
	LoadManager::GetInstance().SetOnComplete([this, &engine, dungeonStageLoader]() {SetStageData(engine, dungeonStageLoader); });
}

void SelectionDungeon::SetStageData(Engine& engine, std::shared_ptr<LoadJSON> setData) {
	// JSONデータの取得
	JSON dungeonData = setData->GetData();
	if (dungeonData.empty()) return;
	// ダンジョンステージデータの宣言
	DungeonStageData stageData;
	stageData.Register(ResourceID::Stage1, dungeonData["Stage"]["StageData"]);
	stageData.Register(ResourceID::StageBone, dungeonData["Stage"]["StageBoneData"]);
	stageData.Register(ResourceID::Player, dungeonData["Character"]["PlayerData"]);
	LoadManager::GetInstance().Clear();
	ActiveDungeon(engine, stageData);
}