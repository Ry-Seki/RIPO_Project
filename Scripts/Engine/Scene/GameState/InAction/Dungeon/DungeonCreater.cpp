/*
 *	@file	DungeonCreater.cpp
 *	@author	Seki
 */

#include "DungeonCreater.h"
#include "../../../../VecMath.h"
#include "../../../../GameObject/GameObjectUtility.h"
#include "../../../../Manager/CameraManager.h"
#include "../../../../Component/Character/CharacterUtility.h"
#include "../../../../Stage/StageUtility.h"
#include "../../../../Stage/StageObject/StageObjectUtility.h"
#include "../../../../Stage/StageObject/Treasure/Treasure.h"
#include "../../../../GameConst.h"

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
	dungeonID = -1;
	entranceDataList.clear();
	dungeonResourceData.ClearResourceData();
	dungeonEntranceData.ClearEntranceData();
}
/*
 *	@brief		ダンジョン生成
 *	@param[in]	int floorID
 *  @param[in]	const std::vector<std::vector<int>>& treasureIDList
 *	@param[in]	int dungeonID
 */
void DungeonCreater::GenerateDungeon(int floorID, const std::vector<std::vector<int>>& treasureIDList, int dungeonID) {
	this->dungeonID = dungeonID;
	// エントランスデータの取得
	dungeonEntranceData.TryGetEntranceData(floorID, entranceDataList);
	// 階段、出口データの分離処理
	SeparateEntranceData();
	// オブジェクトの生成
	int enemyCount = floorData.enemySpawnCount;
	int bossCount = floorData.bossSpawnCount;
	int treasureCount = floorData.treasureSpawnCount;
	int stairCount = floorData.stairSpawnCount;
	int goalCount = floorData.goalSpawnCount;

	// プレイヤー生成
	GeneratePlayer(GameConst::_CREATE_POSNAME_PLAYER, V_ZERO, V_ZERO, { -100, 0, -100 }, { 100,  300,  100 }, { 0, 100, 0 }, { 0,  200,  0 }, 200);
	// カメラ生成
	CameraManager::GetInstance().CreateCamera("camera");
	// 敵の生成
	for (int i = 0; i < enemyCount; i++) {
		GenerateEnemy(GameConst::_CREATE_POSNAME_ENEMY, V_ZERO, { 0, 180 * Deg2Rad, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 100, 0 }, { 0,  400,  0 }, 200, this->dungeonID);
	}
	// ボスの生成
	for (int i = 0; i < bossCount; i++) {
		GenerateBoss(GameConst::_CREATE_POSNAME_BOSS, V_ZERO, { 0, 180 * Deg2Rad, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 100, 0 }, { 0,  500,  0 }, 300, this->dungeonID);
	}
	// お宝の生成処理
	int normalIndex = GameConst::NORMAL_TREASURE_INDEX;
	for (int i = 0, max = treasureIDList[normalIndex].size(); i < max; i++) {
		GenerateTreasure(GameConst::_CREATE_POSNAME_TREASURE, V_ZERO, V_ZERO, 
						 { -100,0,-100 }, { 100,300,100 }, treasureIDList[normalIndex][i]);
	}
	int eventIndex = GameConst::EVENT_TREASURE_INDEX;
	for (int i = 0, max = treasureIDList[eventIndex].size(); i < max; i++) {
		GenerateTreasure(GameConst::_CREATE_POSNAME_TREASURE, V_ZERO, V_ZERO, 
						 { -100,0,-100 }, { 100,300,100 }, treasureIDList[eventIndex][i]);
	}
	// 階段の生成処理
	for (int i = 0; i < stairCount; i++) {
		GenerateStair(stairDataList[i].name,
					  stairDataList[i].position,
					  stairDataList[i].rotation,
					  stairDataList[i].AABBmin,
					  stairDataList[i].AABBmax);
	}
	// 出口の生成処理
	for (int i = 0; i < goalCount; i++) {
		GenerateExit(goalData.name, goalData.position, goalData.rotation, goalData.AABBmin, goalData.AABBmax);
	}

	// ステージハンドルの設定
	int stageHandleCount = dungeonResourceData.stageResource.size();
	std::vector<int> stageHandleList(stageHandleCount);
	for (int i = 0; i < stageHandleCount; i++) {
		stageHandleList[i] = dungeonResourceData.stageResource[i]->GetHandle();
	}
	LoadStage(stageHandleList);
	// ステージボーンデータの設定
	SetStageJSONData(dungeonResourceData.stageBoneResource->GetData());

	// プレイヤーの設定
	// モデルの取得
	int playerHandle = dungeonResourceData.playerResource->GetHandle();
	// プレイヤーオブジェクトの取得
	auto player = GetUseObject(0);
	if (!player) return;
	// 位置の設定
	player->position = GetStartPos();
	// モデルの設定
	SetModelHandle(player.get(), playerHandle);

	// 敵の生成位置の取得
	std::unordered_map<int, Vector3> enemySpawnPos = GetEnemySpwanPos();
	// 敵の取得
	std::vector<GameObjectPtr> enemyList = GetObjectByName(GameConst::_CREATE_POSNAME_ENEMY);
	// モデルハンドルの取得
	int enemyHandle = dungeonResourceData.enemyResource[0]->GetHandle();
	// 敵の設定
	for (int i = 0; i < enemyCount; i++) {
		if (enemyList.size() <= 0) break;
		// 敵の取得
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
		// スポーンIDの設定
		component->SetSpawnEnemyID(i);
	}
	if (bossCount > 0) {
		// ボスの生成位置の取得
		std::unordered_map<int, Vector3> bossSpawnPos = GetEnemySpwanPos();
		// ボスの取得
		std::vector<GameObjectPtr> bossList = GetObjectByName("Boss");
		// モデルハンドルの取得
		int bossHandle = dungeonResourceData.bossResource->GetHandle();
		// ボスの設定
		for (int i = 0; i < bossCount; i++) {
			if (bossList.size() <= 0) break;
			// ボスの取得
			auto bossCharacter = bossList[i];
			if (!bossCharacter) continue;

			bossCharacter->position = bossSpawnPos[0];
			// モデルの設定
			SetModelHandle(bossCharacter.get(), bossHandle);
		}
	}
	if (treasureCount > 0) {
		// お宝の生成位置の取得
		std::unordered_map<int, Vector3> treasureSpawnPos = GetTreasureSpwanPos(floorID);

		// お宝オブジェクトの取得
		GameObjectList treasureList = GetObjectByName(GameConst::_CREATE_POSNAME_TREASURE);
		// お宝の設定
		for (auto& treasure : treasureList) {
			if (!treasure) continue;
			// コンポーネントの取得
			auto component = treasure->GetComponent<StageObjectBase>();
			if (!component) continue;
			// お宝IDの取得
			int treasureID = component->GetTreasureID();
			// 位置の取得
			auto itrPosition = treasureSpawnPos.find(treasureID);
			if (itrPosition == treasureSpawnPos.end()) continue;
			// 位置の設定
			treasure->position = itrPosition->second;
			// モデルハンドルの取得
			auto itrFloor = dungeonResourceData.treasureResource.find(floorID);
			if (itrFloor == dungeonResourceData.treasureResource.end()) continue;
			auto itrModel = itrFloor->second.find(treasureID);
			if (itrModel == itrFloor->second.end()) continue;
			// モデルの設定
			SetModelHandle(treasure.get(), itrModel->second->GetHandle());
		}
	}
	// 階段
	// 階段の取得
	GameObjectList stairList = GetObjectByName(GameConst::_CREATE_POSNAME_STAIR);
	// 階段の遷移先IDデータの取得
	auto stairMoveData = dungeonResourceData.stageFloorResource->GetData();
	// 階段の設定
	// 生成位置の取得
	std::vector<Vector3> stairSpawnPos = GetStairsPos();
	// 階段の設定
	for (int i = 0; i < stairCount; i++) {
		if (stairList.size() <= 0) break;
		// 遷移先IDの取得
		auto stair = stairList[i];
		int stairID = stairDataList[i].moveID;
		if (!stair) continue;
		stair->position = stairSpawnPos[i];
		auto stairComponent = stair->GetComponent<Stair>();
		if (!stairComponent) continue;
		stairComponent->SetStairID(stairID);
	}

	// 出口の設定
	GameObjectList exitList = GetObjectByName(GameConst::_CREATE_POSNAME_GOAL);
	// 生成位置の取得
	std::vector<Vector3> exitSpawnPos = GetGoalPos();
	for (int i = 0; i < goalCount; i++) {
		if (exitList.size() <= 0) break;
		auto exit = exitList[i];
		if (!exit) continue;

		exit->position = exitSpawnPos[i];
	}
}
/*
 *	@brief		ダンジョンの再生成
 *	@param[in]	int floorID
 *  @param[in]	const std::vector<int>& enemySpawnIDList
 *	@param[in]	const int holdTreasureID
 *  @param[in]	GameObjectPtr& holdTreasure
 *  @param[in]	const std::vector<std::vector<int>>& treasureIDList
 */
void DungeonCreater::RegenerateDungeon(int floorID, const std::vector<int>& enemySpawnIDList, const int holdTreasureID,
	const std::vector<std::vector<int>>& treasureIDList) {
	// エントランスデータの取得
	dungeonEntranceData.TryGetEntranceData(floorID, entranceDataList);
	// 階段、出口データの分離処理
	SeparateEntranceData();

	// オブジェクトの再生成
	int enemyCount = floorData.enemySpawnCount;
	int bossCount = floorData.bossSpawnCount;
	int treasureCount = floorData.treasureSpawnCount;
	int stairCount = floorData.stairSpawnCount;
	int goalCount = floorData.goalSpawnCount;

	// 敵の再生成
	for (int i = 0; i < enemyCount; i++) {
		GenerateEnemy(GameConst::_CREATE_POSNAME_ENEMY, V_ZERO, { 0, 180 * Deg2Rad, 0 }, { -100, 0, -100 }, { 100, 300, 100 }, { 0, 100, 0 }, { 0,  400,  0 }, 200, dungeonID);
	}
	// ボスの生成処理
	for (int i = 0; i < bossCount; i++) {
		GenerateBoss(GameConst::_CREATE_POSNAME_BOSS, V_ZERO, { 0, 180 * Deg2Rad, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 100, 0 }, { 0,  500,  0 }, 300, dungeonID);
	}
	// お宝の生成処理
	int normalIndex = GameConst::NORMAL_TREASURE_INDEX;
	for (int i = 0, max = treasureIDList[normalIndex].size(); i < max; i++) {
		if(treasureIDList[normalIndex][i] != holdTreasureID)
		GenerateTreasure(GameConst::_CREATE_POSNAME_TREASURE, V_ZERO, V_ZERO,
						 { -100,0,-100 }, { 100,300,100 }, treasureIDList[normalIndex][i]);
	}
	int eventIndex = GameConst::EVENT_TREASURE_INDEX;
	for (int i = 0, max = treasureIDList[eventIndex].size(); i < max; i++) {
		if (treasureIDList[eventIndex][i] != holdTreasureID)
		GenerateTreasure(GameConst::_CREATE_POSNAME_TREASURE, V_ZERO, V_ZERO,
						 { -100,0,-100 }, { 100,300,100 }, treasureIDList[eventIndex][i]);
	}	
	// 階段の生成処理
	for (int i = 0; i < stairCount; i++) {
		GenerateStair(stairDataList[i].name,
					  stairDataList[i].position,
					  stairDataList[i].rotation,
					  stairDataList[i].AABBmin,
					  stairDataList[i].AABBmax);
	}
	// 出口の生成処理
	for (int i = 0; i < goalCount; i++) {
		GenerateExit(goalData.name, goalData.position, goalData.rotation, goalData.AABBmin, goalData.AABBmax);
	}
	// オブジェクトの設定
	Vector3 respawnPos = GetRespawnPos();
	// プレイヤー
	// プレイヤーオブジェクトの取得
	auto player = GetUseObject(0);
	if (!player) return;
	// 位置の設定
	player->position = respawnPos;

	// 敵
	std::unordered_map<int, Vector3> enemySpawnPos = GetEnemySpwanPos();
	// 敵の取得
	GameObjectList enemyList = GetObjectByName(GameConst::_CREATE_POSNAME_ENEMY);
	// モデルハンドルの取得
	int enemyHandle = dungeonResourceData.enemyResource[0]->GetHandle();
	// 敵の設定
	for (int i = 0; i < enemyCount; i++) {
		if (enemyList.size() <= 0) break;
		auto enemyCharacter = enemyList[i];
		if (!enemyCharacter) continue;
		// モデルの設定
		SetModelHandle(enemyCharacter.get(), enemyHandle);
		// コンポーネントの取得
		std::shared_ptr<EnemyComponent> component = enemyCharacter->GetComponent<EnemyComponent>();
		if (!component) continue;
		// 位置の設定
		int spawnID;
		// enemySpawnIDListのサイズで変更する
		if (enemySpawnIDList.size() > 0) {
			spawnID = enemySpawnIDList[i];
		} else {
			spawnID = i;
		}
		enemyCharacter->position = enemySpawnPos[spawnID];
		enemyCharacter->scale = { 4.5f, 4.5f, 4.5f };
		// WayPointの設定
		component->SetWayPoint(enemyCharacter->position);
		component->SetSpawnEnemyID(spawnID);
	}
	if (bossCount > 0) {
		// ボスの生成位置の取得
		std::unordered_map<int, Vector3> bossSpawnPos = GetEnemySpwanPos();
		// ボスの取得
		std::vector<GameObjectPtr> bossList = GetObjectByName("Boss");
		// モデルハンドルの取得
		int bossHandle = dungeonResourceData.bossResource->GetHandle();
		// ボスの設定
		for (int i = 0; i < bossCount; i++) {
			if (bossList.size() <= 0) break;
			// ボスの取得
			auto bossCharacter = bossList[i];
			if (!bossCharacter) continue;

			bossCharacter->position = bossSpawnPos[0];
			bossCharacter->scale = { 3, 3, 3 };
			// モデルの設定
			SetModelHandle(bossCharacter.get(), bossHandle);
		}
	}
	if (treasureCount > 0) {
		// お宝の生成位置の取得
		std::unordered_map<int, Vector3> treasureSpawnPos = GetTreasureSpwanPos(floorID);
		// お宝オブジェクトの取得
		GameObjectList treasureList = GetObjectByName(GameConst::_CREATE_POSNAME_TREASURE);
		// お宝の設定
		for (auto& treasure : treasureList) {
			if (!treasure) continue;
			// コンポーネントの取得
			auto component = treasure->GetComponent<StageObjectBase>();
			if (!component) continue;
			// プレイヤーが持っているお宝か判定
			if (!component->IsHold()) {
				// 持っているお宝IDと生成されたお宝IDが同じ場合は、それを削除する
				if (component->GetTreasureID() == holdTreasureID) {
					RemoveStageObject(treasure->ID);
					continue;
				}else {
					// お宝IDの取得
					int treasureID = component->GetTreasureID();
					// 位置の取得
					auto itrPosition = treasureSpawnPos.find(treasureID);
					if (itrPosition == treasureSpawnPos.end()) continue;
					// 位置の設定
					treasure->position = itrPosition->second;
					// モデルハンドルの取得
					auto itrFloor = dungeonResourceData.treasureResource.find(floorID);
					if (itrFloor == dungeonResourceData.treasureResource.end()) continue;
					auto itrModel = itrFloor->second.find(treasureID);
					if (itrModel == itrFloor->second.end()) continue;
					// モデルの設定
					SetModelHandle(treasure.get(), itrModel->second->GetHandle());
				}
			}
		}
	}

	// 階段
	// 階段の取得
	GameObjectList stairList = GetObjectByName(GameConst::_CREATE_POSNAME_STAIR);
	// 階段の遷移先IDデータの取得
	auto stairMoveData = dungeonResourceData.stageFloorResource->GetData();
	// 生成位置の取得
	std::vector<Vector3> stairSpawnPos = GetStairsPos();
	// 階段の設定
	for (int i = 0; i < stairCount; i++) {
		if (stairList.size() <= 0) break;
		// 遷移先IDの取得
		auto stair = stairList[i];
		int stairID = stairDataList[i].moveID;
		if (!stair) continue;

		stair->position = stairSpawnPos[i];
		auto stairComponent = stair->GetComponent<Stair>();
		if (!stairComponent) continue;

		stairComponent->SetStairID(stairID);
	}

	// 出口の設定
	GameObjectList exitList = GetObjectByName(GameConst::_CREATE_POSNAME_GOAL);
	// 生成位置の取得
	std::vector<Vector3> exitSpawnPos = GetGoalPos();
	for (int i = 0; i < goalCount; i++) {
		if (exitList.size() <= 0) break;
		auto exit = exitList[i];
		if (!exit) continue;

		exit->position = exitSpawnPos[i];
	}
}
/*
 *	@brief	階段、出口のデータの設定
 */
void DungeonCreater::SeparateEntranceData() {
	for (const auto data : entranceDataList) {
		if (data.name == GameConst::_CREATE_POSNAME_STAIR) {
			stairDataList.push_back(data);
		}else if(data.name == GameConst::_CREATE_POSNAME_GOAL){
			goalData = data;
		}
	}
}
