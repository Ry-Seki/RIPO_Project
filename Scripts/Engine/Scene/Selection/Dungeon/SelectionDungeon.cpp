/*
 *	@file	SelectionDungeon.cpp
 *	@author	Seki
 */

#include "SelectionDungeon.h"
#include "../../../Load/LoadManager.h"
#include "../../DayAction/ActionManager.h"
#include "../../Selection/SelectionManager.h"
#include "../../../Audio/AudioUtility.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"

#include "../../../GameEnum.h"

/*
 *	@brief	初期化処理
 */
void SelectionDungeon::Initialize(Engine& engine) {
	LoadManager& load = LoadManager::GetInstance();
	isComplete = false;
	dungeonDataLoader = load.LoadResource<DungeonDataLoader>("Data/Dungeon/DungeonList.csv");
	load.SetOnComplete( [this, &engine]() { SetupData(engine); } );
}
/*
 *	@brief	準備前処理
 */
void SelectionDungeon::Setup(Engine& engine) {
}
/*
 *	@brief	ロード済みデータのセット (コールバック処理)
 */
void SelectionDungeon::SetupData(Engine& engine) {
	dungeonDataList = dungeonDataLoader->dungeonList;
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade);
}
/*
 *	@brief	更新処理
 */
void SelectionDungeon::Update(Engine& engine, float deltaTime) {
	if (dungeonDataList.empty() || isComplete) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_0)) { 
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		dungeonID = 0;
		inputHandle = true;
		isComplete = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [&engine, this]() {
			StartStageDataLoad(engine, dungeonID);
		});
	}
	if (CheckHitKey(KEY_INPUT_0) == 0) inputHandle = false;
}
/*
 *	@brief	描画処理
 */
void SelectionDungeon::Render() {
	DrawFormatString(50, 50, GetColor(0, 0, 0), "0: TutorialDungeon");
}
/*
 *  @brief  行動実行関数の呼び出し
 */
void SelectionDungeon::SetAction() {

}
/*
 *	@brief	ダンジョンステージデータの読み込み
 */
void SelectionDungeon::StartStageDataLoad(Engine& engine, int dungeonID) {
	LoadManager& load = LoadManager::GetInstance();
	DungeonData dungeonData = dungeonDataList[dungeonID];
	// ファイルパスの取得
	std::string filePath = dungeonData.dungeonPath;
	if (filePath.empty()) return;
	// ダンジョンステージデータの読み込み
	std::vector<std::shared_ptr<LoadJSON>> jsonList;
	jsonList.push_back(load.LoadResource<LoadJSON>(filePath));
	jsonList.push_back(load.LoadResource<LoadJSON>(_DUNGEON_FLOOR_PATH));
	// コールバック登録
	load.SetOnComplete([this, &engine, jsonList]() {SetStageData(engine, jsonList); });
}
/*
 *	@brief		読み込んだステージデータをセットし、アクションマネージャーに渡す(コールバック)
 *  @param[in]	const std::vector<std::shared_ptr<LoadJSON>>& setDataList	ロードしたJSONデータリスト
 */
void SelectionDungeon::SetStageData(Engine& engine, const std::vector<std::shared_ptr<LoadJSON>>& setDataList) {
	// JSONデータの取得
	JSON dungeonData = setDataList[0]->GetData();
	JSON dungeonfloorData = setDataList[1]->GetData();
	if (dungeonData.empty() || dungeonfloorData.empty()) return;
	// ダンジョンステージデータの宣言
	DungeonStageData stageData;
	// JSONデータを再帰的に登録
	stageData.LoadFromJSON(dungeonData);
	// ダンジョンフロアデータの宣言
	DungeonFloorData floorData;
	// JSONデータの登録
	floorData.LoadFromJSON(dungeonfloorData, 1);
	ActiveDungeon(engine, stageData, floorData);
}