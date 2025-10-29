/*
 *	@file	SelectionDungeon.cpp
 *	@author	Seki
 */

#include "SelectionDungeon.h"
#include "../../../Load/LoadManager.h"
#include "../../DayAction/ActionManager.h"
#include "../../Selection/SelectionManager.h"

/*
 *	初期化処理
 */
void SelectionDungeon::Initialize(Engine& engine) {
	LoadManager& load = LoadManager::GetInstance();
	isComplete = false;
	dungeonDataLoader = load.LoadResource<DungeonDataLoader>("Data/Dungeon/DungeonList.csv");
	load.SetOnComplete( [this, &engine]() { Setup(engine); } );
}
/*
 *	ロード済みデータのコールバック処理
 */
void SelectionDungeon::Setup(Engine& engine) {
	dungeonDataList = dungeonDataLoader->dungeonList;
}
/*
 *	更新処理
 */
void SelectionDungeon::Update(Engine& engine, float deltaTime) {
	if (dungeonDataList.empty() || isComplete) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_0)) { 
		inputHandle = true;
		StartStageDataLoad(engine, 0);
	}

	if (CheckHitKey(KEY_INPUT_0) == 0) inputHandle = false;
}
/*
 *	描画処理
 */
void SelectionDungeon::Render() {
	DrawFormatString(50, 50, GetColor(0, 0, 0), "0: TutorialDungeon");
}
/*
 *	ダンジョンステージデータの読み込み
 */
void SelectionDungeon::StartStageDataLoad(Engine& engine, int dungeonID) {
	LoadManager& load = LoadManager::GetInstance();
	DungeonData dungeonData = dungeonDataList[dungeonID];
	// ファイルパスの取得
	std::string filePath = dungeonData.dungeonPath;
	if (filePath.empty()) return;
	// ダンジョンステージデータの読み込み
	auto dungeonStageLoader = load.LoadResource<LoadJSON>(filePath);
	// コールバック登録
	load.SetOnComplete([this, &engine, dungeonStageLoader]() {SetStageData(engine, dungeonStageLoader); });
}
/*
 *	ロード済みデータのコールバック処理
 */
void SelectionDungeon::SetStageData(Engine& engine, std::shared_ptr<LoadJSON> setData) {
	// JSONデータの取得
	JSON dungeonData = setData->GetData();
	if (dungeonData.empty()) return;
	// ダンジョンステージデータの宣言
	DungeonStageData stageData;
	// JSONデータを再帰的に登録
	stageData.LoadFromJson(dungeonData);
	ActiveDungeon(engine, stageData);
	isComplete = true;
}