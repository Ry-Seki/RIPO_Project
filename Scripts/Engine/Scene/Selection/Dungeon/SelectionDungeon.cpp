/*
 *	@file	SelectionDungeon.cpp
 *	@author	Seki
 */

#include "SelectionDungeon.h"
#include "../../../Load/LoadManager.h"
#include "../../DayAction/ActionManager.h"
#include "../../Selection/SelectionManager.h"


void SelectionDungeon::Initialize(Engine& engine) {
	dungeonDataLoader = std::make_shared<DungeonDataLoader>("Data/Dungeon/DungeonList.csv");
	LoadManager::GetInstance().AddLoader(dungeonDataLoader);
	LoadManager::GetInstance().SetOnComplete( [this, &engine]() { Setup(engine); } );
}

void SelectionDungeon::Setup(Engine& engine) {
	dungeonDataList = dungeonDataLoader->dungeonList;
	isActive = true;
}

void SelectionDungeon::Update(Engine& engine, float deltaTime) {
	if (dungeonDataList.empty() && !isActive) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_1)) { 
		DebugStageLoad(engine, 0);
		inputHandle = true;
	}

	if (CheckHitKey(KEY_INPUT_1) == 0) inputHandle = false;
}

void SelectionDungeon::Render() {

}

void SelectionDungeon::StartStageDataLoad(int dungeonID) {
	DungeonData dungeonData = dungeonDataList[dungeonID];
	// ファイルパスの取得
	std::string filePath = dungeonData.dungeonPath;
	if (filePath.empty()) return;
	// ダンジョンステージデータの読み込み
	auto dungeonStageLoader = std::make_shared<LoadJSON>(filePath);
	LoadManager::GetInstance().AddLoader(dungeonStageLoader);
	// コールバック登録
	LoadManager::GetInstance().SetOnComplete([this, dungeonStageLoader]() {SetStageData(dungeonStageLoader); });
}

void SelectionDungeon::SetStageData(std::shared_ptr<LoadJSON> setData) {
	
}

void SelectionDungeon::DebugStageLoad(Engine& engine, int dungeonID) {
	DungeonData dungeonData = dungeonDataList[dungeonID];
	// ファイルパスの取得
	std::string filePath = dungeonData.dungeonPath;
	if (filePath.empty()) return;
	// ダンジョンステージデータの読み込み
	auto dungeonStageLoader = std::make_shared<LoadJSON>(filePath);
	LoadManager::GetInstance().AddLoader(dungeonStageLoader);
	// コールバック登録
	LoadManager::GetInstance().SetOnComplete([this, &engine, dungeonStageLoader]() {DebugSetStageData(engine, dungeonStageLoader); });
}

void SelectionDungeon::DebugSetStageData(Engine& engine, std::shared_ptr<LoadJSON> setData) {
	JSON dungeonData = setData->GetData();
	std::string dungeonPath = dungeonData["StageData"];

	ActionManager::GetInstance().DebugActiveDungeon(engine, dungeonPath);
}
