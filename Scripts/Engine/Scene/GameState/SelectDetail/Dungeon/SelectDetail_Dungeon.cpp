/*
 *	@file	SelectDetail_Dungeon.cpp
 *	@author	Seki
 */

#include "SelectDetail_Dungeon.h"
#include "../SelectDetailBase.h"
#include "../GameState_SelectDetail.h"
#include "../../GameStateMachine.h"
#include "../../../../Load/LoadManager.h"
#include "../../../../Load/Dungeon/DungeonDataLoader.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Audio/AudioUtility.h"
#include "../../../../Load/JSON/LoadJSON.h"
#include "../../../../JSON.h"
#include "../../../../../Data/Dungeon/DungeonStageData.h"
#include "../../../../../Data/Dungeon/DungeonFloorData.h"


/*
 *	@brief	初期化処理
 */
void SelectDetail_Dungeon::Initialize() {
	LoadManager& load = LoadManager::GetInstance();
	dungeonDataLoader = load.LoadResource<DungeonDataLoader>("Data/Dungeon/DungeonList.csv");
	load.SetOnComplete([this]() { SetupData(); });
}
/*
 *	@brief	ロード済みデータのセット
 */
void SelectDetail_Dungeon::SetupData() {
	dungeonDataList = dungeonDataLoader->dungeonList;

}
/*
 *	@brief	準備前処理
 */
void SelectDetail_Dungeon::Setup() {
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade);
}
/*
 *	@brief	更新処理
 */
void SelectDetail_Dungeon::Update(float deltaTime) {
	if (dungeonDataList.empty()) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_1)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		// TODO:ここはしっかりと治す
		dungeonID = 0;
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			StartDungeonDataLoad(dungeonID);
		});
	}
	if (CheckHitKey(KEY_INPUT_1) == 0) inputHandle = false;
}
/*
 *	@brief	描画処理
 */
void SelectDetail_Dungeon::Render() {
	DrawFormatString(50, 50, GetColor(0, 0, 0), "1: TutorialDungeon");
}
/*
 *	@brief	片付け処理
 */
void SelectDetail_Dungeon::Teardown() {
	inputHandle = false;
}
/*
 *	@brief	ダンジョンステージデータの読み込み
 */
void SelectDetail_Dungeon::StartDungeonDataLoad(int dungeonID) {
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
	load.SetOnComplete([this, jsonList]() {SetDungeonData(jsonList); });
}
/*
 *	@brief		読み込んだステージデータをセットし、アクションマネージャーに渡す(コールバック)
 *  @param[in]	const std::vector<std::shared_ptr<LoadJSON>>& setDataList	ロードしたJSONデータリスト
 */
void SelectDetail_Dungeon::SetDungeonData(const std::vector<std::shared_ptr<LoadJSON>>& setDataList) {
	// JSONデータの取得
	JSON dungeonData = setDataList[0]->GetData();
	JSON dungeonfloorData = setDataList[1]->GetData();
	if (dungeonData.empty() || dungeonfloorData.empty()) return;

	auto& ctx = owner->GetOwner()->GetActionContext();
	// それぞれのデータを初期化
	ctx.dungeonStageData.LoadFromJSON(dungeonData);
	ctx.dungeonFloorData.LoadFromJSON(dungeonfloorData, dungeonID);
	// ステートの切り替え
	owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
}