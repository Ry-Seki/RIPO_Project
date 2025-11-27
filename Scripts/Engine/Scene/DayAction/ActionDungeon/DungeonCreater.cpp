/*
 *	@file	DungeonCreater.cpp
 *	@author	Seki
 */

#include "DungeonCreater.h"
#include "../../../GameObject/GameObjectUtility.h"
#include "../../../Manager/CameraManager.h"
#include "../../../Component/Character/CharacterUtility.h"
#include "../../../Stage/StageUtility.h"
#include "../../../Stage/StageObject/StageObjectUtility.h"
#include "../../../GameConst.h"

using namespace GameObjectUtility;
using namespace CharacterUtility;
using namespace StageUtility;
using namespace StageObjectUtility;

/*
 *	@brief		初期化処理
 */
void DungeonCreater::Initialize() {
}
/*
 *	@brief		準備前処理
 */
void DungeonCreater::Setup() {
	isFirst = true;
}
/*
 *	@brief		ダンジョン生成
 *	@param[in]	int floorID
 *  @param[in]	DungeonFloorData& floorData
 *  @param[in]	DungeonResourceData& resourceData
 *  @param[in]	const std::vector<int>& treasureIDList
 *  @param[out]	int& stairID
 */
void DungeonCreater::RegenerateDungeon(
	int floorID, DungeonFloorData& floorData, DungeonResourceData& resourceData, const std::vector<int>& treasureIDList, int& stairID) {
	// フロアデータの取得
	FloorData spawnData;
	floorData.TryGetFloorData(floorID, spawnData);

	// オブジェクトの生成
	// 初回生成時のみ、プレイヤーとカメラ、ステージモデル生成
	if (isFirst) {
		isFirst = false;
		// プレイヤー生成
		GeneratePlayer(GameConst::_CREATE_POSNAME_PLAYER, V_ZERO, V_ZERO, { -100, 0, -100 }, { 100,  300,  100 }, { 0, 100, 0 }, { 0,  200,  0 }, 200);
		// カメラ生成
		CameraManager::GetInstance().CreateCamera("camera", { 0, 0, 0 }, { 0, 0, 0 });
		// ステージハンドルの設定
		int stageHandleCount = resourceData.stageResource.size();
		std::vector<int> stageHandleList(stageHandleCount);
		for (int i = 0; i < stageHandleCount; i++) {
			stageHandleList[i] = resourceData.stageResource[i]->GetHandle();
		}
		LoadStage(stageHandleList);
	}

	// 敵の生成
	for (int i = 0; i < spawnData.enemySpawnCount; i++) {
		GenerateEnemy(GameConst::_CREATE_POSNAME_ENEMY, { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 }, { 0, 100, 0 }, { 0,  200,  0 }, 200);
	}
	// お宝の生成処理
	for (int i = 0; i < spawnData.treasureSpawnCount; i++) {
		GenerateTreasure(GameConst::_CREATE_POSNAME_TREASURE, { 0,0,0 }, { 0,0,0 }, { -100,0,-100 }, { 100,300,100 }, treasureIDList[i]);
	}
	// 階段の生成処理
	for (int i = 0; i < spawnData.stairSpawnCount; i++) {
		GenerateStair("Stair", { 0,0,0 }, { 0,0,0 }, { -500,-500,-10 }, { 500,800,10 });
	}
	// 出口の生成処理
	for (int i = 0; i < spawnData.goalSpawnCount; i++) {
		GenerateExit("Exit", { 0,0,0 }, { 0,0,0 }, { -1000,-700,-10 }, { 1000,700,10 });
	}

	// ステージボーンデータの設定
	// TODO : スポーンデータの修正
	SetStageJSONData(resourceData.stageBoneResource[0]->GetData());

	// プレイヤーの設定
	// モデルの取得
	int playerHandle = resourceData.playerResource->GetHandle();
	// プレイヤーオブジェクトの取得
	auto player = GetUseObject(0);
	if (!player) return;
	// 位置の設定
	player->position = GetStartPos();
	// モデルの設定
	SetModelHandle(player.get(), playerHandle);

	// 敵の設定
	// 敵の生成位置の取得
	std::vector<int> enemySpawnIDList;
	std::vector<Vector3> enemySpawnPos = GetEnemySpwanPos(enemySpawnIDList);
	// 敵の取得
	std::vector<GameObjectPtr> enemyList = GetObjectByName(GameConst::_CREATE_POSNAME_ENEMY);
	// モデルハンドルの取得
	int enemyHandle = resourceData.enemyResource[0]->GetHandle();
	// 敵の取得
	for (int i = 0, max = enemyList.size(); i < max; i++) {
		auto enemyCharacter = enemyList[i];
		if (!enemyCharacter) continue;
		// 位置の設定
		enemyCharacter->position = enemySpawnPos[i];
		enemyCharacter->scale = { 4.5f, 4.5f, 4.5f };
		// モデルの設定
		SetModelHandle(enemyCharacter.get(), enemyHandle);
		// コンポーネントの取得
		std::shared_ptr<EnemyComponent> component = enemyCharacter->GetComponent<EnemyComponent>();
		if (!component) continue;
		// WayPointの設定
		component->SetWayPoint(enemyCharacter->position);
	}

	// 宝の設定
	// お宝の生成位置の取得
	std::vector<Vector3> treasureSpawnPos = GetTreasureSpwanPos();
	// ハンドルの要素数の取得
	size_t treasureTypeCount = resourceData.treasureResource.size();
	GameObjectList treasureList = GetObjectByName(GameConst::_CREATE_POSNAME_TREASURE);
	// お宝の取得
	for (int i = 0, max = treasureList.size(); i < max; i++) {
		// モデルハンドルの取得
		int treasureHandle = resourceData.treasureResource[i]->GetHandle();
		// 宝オブジェクトの取得
		GameObject* treasure = treasureList[i].get();
		if (!treasure) continue;
		// 位置の設定
		treasure->position = treasureSpawnPos[i];
		// モデルの設定
		SetModelHandle(treasure, treasureHandle);
	}
	// 階段の設定
	auto stair = GetStageObject("Stair");
	auto stairMoveData = resourceData.stageFloorResource->GetData();
	// TODO: この数字を生成される階段の数にする
	stairID = stairMoveData[std::to_string(floorID)]["0"];
	if (stair) {
		Vector3 stairSpawnPos = GetStairsPos();
		stair->position = stairSpawnPos;
		auto stairComponent = stair->GetComponent<Stair>();
		if (stairComponent) 
			stairComponent->SetStairID(stairID);
	}
	// 出口の設定
	auto exit = GetStageObject("Exit");
	if (exit) {
		Vector3 exitSpawnPos = GetGoalPos();
		exit->position = exitSpawnPos;
	}
}
