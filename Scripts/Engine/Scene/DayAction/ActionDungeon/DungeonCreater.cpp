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
#include "../../../Stage/StageObject/Treasure/Treasure.h"
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
	
}
/*
 *	@brief		ダンジョン生成
 *	@param[in]	int floorID
 *  @param[in]	DungeonFloorData& floorData
 *  @param[in]	DungeonResourceData& resourceData
 *  @param[in]	const std::vector<int>& treasureIDList
 *  @param[out]	int& stairID
 */
void DungeonCreater::GenerateDungeon(
	int floorID, DungeonFloorData& floorData, DungeonResourceData& resourceData, 
	const std::vector<int>& treasureIDList, int& stairID) {
	// フロアデータの取得
	FloorData spawnData;
	floorData.TryGetFloorData(floorID, spawnData);

	// オブジェクトの生成
	int enemyCount = d_floorData.enemySpawnCount;
	int bossCount = d_floorData.bossSpawnCount;
	int treasureCount = d_floorData.treasureSpawnCount;
	int stairCount = d_floorData.stairSpawnCount;
	int goalCount = d_floorData.goalSpawnCount;

	// プレイヤー生成
	GeneratePlayer(GameConst::_CREATE_POSNAME_PLAYER, V_ZERO, V_ZERO, { -100, 0, -100 }, { 100,  300,  100 }, { 0, 100, 0 }, { 0,  200,  0 }, 200);
	// カメラ生成
	CameraManager::GetInstance().CreateCamera("camera");
	// 敵の生成
	for (int i = 0; i < enemyCount; i++) {
		GenerateEnemy(GameConst::_CREATE_POSNAME_ENEMY, V_ZERO, V_ZERO, { -100, 0, -100 }, { 100, 300, 100 }, { 0, 100, 0 }, { 0,  200,  0 }, 200);
	}
	// TODO : ボスの生成
	
	// お宝の生成処理
	for (int i = 0; i < treasureCount; i++) {
		GenerateTreasure(GameConst::_CREATE_POSNAME_TREASURE, V_ZERO, V_ZERO, { -100,0,-100 }, { 100,300,100 }, treasureIDList[i]);
	}
	// 階段の生成処理
	for (int i = 0; i < stairCount; i++) {
		GenerateStair("Stair", V_ZERO, V_ZERO, { -500,-500,-10 }, { 500,800,10 });
	}
	// 出口の生成処理
	for (int i = 0; i < goalCount; i++) {
		GenerateExit("Exit", V_ZERO, V_ZERO, { -1000,-700,-10 }, { 1000,700,10 });
	}

	// ステージハンドルの設定
	int stageHandleCount = resourceData.stageResource.size();
	std::vector<int> stageHandleList(stageHandleCount);
	for (int i = 0; i < stageHandleCount; i++) {
		stageHandleList[i] = resourceData.stageResource[i]->GetHandle();
	}
	LoadStage(stageHandleList);
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

	// 敵の生成ID
	std::vector<int> enemySpawnIDList;
	// 敵の生成位置の取得
	std::vector<Vector3> enemySpawnPos = GetEnemySpwanPos(enemySpawnIDList);
	// 敵の取得
	std::vector<GameObjectPtr> enemyList = GetObjectByName(GameConst::_CREATE_POSNAME_ENEMY);
	// モデルハンドルの取得
	int enemyHandle = resourceData.enemyResource[0]->GetHandle();
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

	// 宝の設定
	// お宝の生成位置の取得
	std::vector<Vector3> treasureSpawnPos = GetTreasureSpwanPos();
	// ハンドルの要素数の取得
	size_t treasureTypeCount = resourceData.treasureResource.size();
	GameObjectList treasureList = GetObjectByName(GameConst::_CREATE_POSNAME_TREASURE);
	// お宝の取得
	for (int i = 0; i < treasureCount; i++) {
		if (treasureList.size() <= 0) break;
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
	// 階段
	// 階段の取得
	GameObjectList stairList = GetObjectByName("Stair");
	// 階段の遷移先IDデータの取得
	auto stairMoveData = d_resourceData.stageFloorResource->GetData();
	// 階段の設定
	for (int i = 0; i < stairCount; i++) {
		if (stairList.size() <= 0) break;
		// 遷移先IDの取得
		auto stair = stairList[i];
		stairID = stairMoveData[std::to_string(floorID)][std::to_string(i)];
		if (stair) {
			Vector3 stairSpawnPos = GetStairsPos();
			stair->position = stairSpawnPos;
			auto stairComponent = stair->GetComponent<Stair>();
			if (stairComponent) stairComponent->SetStairID(stairID);
		}
	}
	// 出口の設定
	auto exit = GetStageObject("Exit");
	if (exit) {
		Vector3 exitSpawnPos = GetGoalPos();
		exit->position = exitSpawnPos;
	}
}
/*
 *	@brief		ダンジョンの再生成
 *	@param[in]	int floorID
 *	@param[in]	const int holdTreasureID
 *  @param[in]	GameObjectPtr& holdTreasure
 *  @param[in]	const std::vector<int>& treasureIDList
 *  @param[out]	int& stairID
 */
void DungeonCreater::RegenerateDungeon(int floorID, GameObjectList& setEnemyList, const int holdTreasureID, 
									   const std::vector<int>& treasureIDList, int& stairID) {
	// オブジェクトの再生成
	int enemyCount = d_floorData.enemySpawnCount;
	int bossCount = d_floorData.bossSpawnCount;
	int treasureCount = d_floorData.treasureSpawnCount;
	int stairCount = d_floorData.stairSpawnCount;
	int goalCount = d_floorData.goalSpawnCount;

	// 敵の再生成
	for (int i = 0; i < enemyCount; i++) {
		GenerateEnemy(GameConst::_CREATE_POSNAME_ENEMY, V_ZERO, V_ZERO, { -100, 0, -100 }, { 100, 300, 100 }, { 0, 100, 0 }, { 0,  200,  0 }, 200);
	}
	// TODO : ボスの生成処理
	
	// お宝の生成処理
	for (int i = 0; i < treasureCount; i++) {
		GenerateTreasure(GameConst::_CREATE_POSNAME_TREASURE, V_ZERO, V_ZERO, { -100,0,-100 }, { 100,300,100 }, treasureIDList[i]);
	}
	// 階段の生成処理
	for (int i = 0; i < stairCount; i++) {
		GenerateStair("Stair", V_ZERO, V_ZERO, { -500,-500,-10 }, { 500,800,10 });
	}
	// 出口の生成処理
	for (int i = 0; i < goalCount; i++) {
		GenerateExit("Exit", V_ZERO, V_ZERO, { -1000,-700,-10 }, { 1000,700,10 });
	}
	// オブジェクトの設定
	// プレイヤー
	// プレイヤーオブジェクトの取得
	auto player = GetUseObject(0);
	if (!player) return;
	// 位置の設定
	player->position = GetStartPos();
	
	// 敵
	// 敵の生成位置の取得
	std::vector<int> enemySpawnIDList;
	std::vector<Vector3> enemySpawnPos = GetEnemySpwanPos(enemySpawnIDList);
	// 敵の取得
	GameObjectList originEnemyList = setEnemyList;
	GameObjectList enemyList = GetObjectByName(GameConst::_CREATE_POSNAME_ENEMY);
	// 階層移動前の敵がいれば、生成した敵の情報を上書きする
	if (originEnemyList.size() > 0) {
		for (int i = 0; i < enemyCount; i++) {
			enemyList[i] = originEnemyList[i];
		}
	}
	// 敵の設定
	for (int i = 0, max = enemyList.size(); i < max; i++) {
		auto enemyCharacter = enemyList[i];
		if (!enemyCharacter) continue;
		// コンポーネントの取得
		std::shared_ptr<EnemyComponent> component = enemyCharacter->GetComponent<EnemyComponent>();
		if (!component) continue;
		// 位置の設定
		int spawnPos = component->GetSpawnEnemyID();
		enemyCharacter->position = enemySpawnPos[spawnPos];
		enemyCharacter->scale = { 4.5f, 4.5f, 4.5f };
		// WayPointの設定
		component->SetWayPoint(enemyCharacter->position);
	}

	// お宝
	// お宝の生成位置の取得
	std::vector<Vector3> treasureSpawnPos = GetTreasureSpwanPos();
	GameObjectList treasureList = GetObjectByName(GameConst::_CREATE_POSNAME_TREASURE);
	// お宝の取得
	for (int i = 0; i < treasureCount; i++) {
		if (treasureList.size() <= 0) break;
		// 宝オブジェクトの取得
		auto treasure = treasureList[i].get();
		if (!treasure) continue;
		// お宝コンポーネントの取得
		auto component = treasure->GetComponent<StageObjectBase>();
		if (!component) continue;
		// 持っているお宝IDと生成されたお宝IDが同じ場合は、それを削除する
		if (component->GetTreasureID() == holdTreasureID) {
			RemoveStageObject(treasure->ID);
			continue;
		} else {
			// 位置の設定
			treasure->position = treasureSpawnPos[i];
		}
	}

	// 階段
	// 階段の取得
	GameObjectList stairList = GetObjectByName("Stair");
	// 階段の遷移先IDデータの取得
	auto stairMoveData = d_resourceData.stageFloorResource->GetData();
	// 階段の設定
	for (int i = 0; i < stairCount; i++) {
		if (stairList.size() <= 0) break;
		// 遷移先IDの取得
		auto stair = stairList[i];
		stairID = stairMoveData[std::to_string(floorID)][std::to_string(i)];
		if (stair) {
			Vector3 stairSpawnPos = GetStairsPos();
			stair->position = stairSpawnPos;
			auto stairComponent = stair->GetComponent<Stair>();
			if (stairComponent) stairComponent->SetStairID(stairID);
		}
	}
	// 出口の設定
	auto exit = GetStageObject("Exit");
	if (exit) {
		Vector3 exitSpawnPos = GetGoalPos();
		exit->position = exitSpawnPos;
	}
}
