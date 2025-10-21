/*
 *	@file	SelectionDungeon.cpp
 *	@author	Seki
 */

#include "SelectionDungeon.h"
#include "../../../Load/LoadManager.h"
#include "../../DayAction/ActionManager.h"


void SelectionDungeon::Initialize() {
	dungeonDataLoader = std::make_shared<DungeonDataLoader>("Data/Dungeon/DungeonList.csv");
	LoadManager::GetInstance().AddLoader(dungeonDataLoader);
	LoadManager::GetInstance().SetOnComplete( [this]() { Setup(); } );
}

void SelectionDungeon::Setup() {
	dungeonDataList = dungeonDataLoader->dungeonList;
	isActive = true;
}

void SelectionDungeon::Update(float deltaTime) {
	if (dungeonDataList.empty() && !isActive) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_1)) { 
		DebugStageLoad(0);
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

void SelectionDungeon::DebugStageLoad(int dungeonID) {
	DungeonData dungeonData = dungeonDataList[dungeonID];
	// ファイルパスの取得
	std::string filePath = dungeonData.dungeonPath;
	if (filePath.empty()) return;
	// ダンジョンステージデータの読み込み
	auto dungeonStageLoader = std::make_shared<LoadJSON>(filePath);
	LoadManager::GetInstance().AddLoader(dungeonStageLoader);
	// コールバック登録
	LoadManager::GetInstance().SetOnComplete([this, dungeonStageLoader]() {DebugSetStageData(dungeonStageLoader); });
}

void SelectionDungeon::DebugSetStageData(std::shared_ptr<LoadJSON> setData) {

}
