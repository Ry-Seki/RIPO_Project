/*
 *	@file	DungeonFloorData.h
 *	@author	Seki
 */

#ifndef _DUNGEON_FLOOR_DATA_H_
#define _DUNGEON_FLOOR_DATA_H_

#include "../../Engine/JSON.h"

#include <unordered_map>
#include <vector>

/*
 *	@brief	階層データ
 */
struct FloorData {
	bool isFirst = true;			// 最初にその階層に入ったか判別フラグ
	int enemySpawnCount = 0;		// 敵の生成数
	int bossSpawnCount = 0;			// ボスの生成数
	int treasureSpawnCount = 0;		// お宝生成数
	int stairSpawnCount = 0;		// 階段生成数
	int goalSpawnCount = 0;			// 出口生成数
};

/*
 *	@brief	ダンジョン階層データ
 */
class DungeonFloorData {
private:
	std::unordered_map<int, FloorData> dungeonFloorMap;

public:
	/*
	 *	@brief	コンストラクタ
	 */
	DungeonFloorData() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~DungeonFloorData() = default;

public:
	/*
	 *  @brief      JSONデータの読み込み、mapに自動で登録する
	 *  @param[in]  const JSON& setJSON     読み込むJSONデータ
	 *  @param[in]	int dungeonID			ダンジョンID
	 */
	void LoadFromJSON(const JSON& setJSON, int dungeonID) {
		dungeonFloorMap.clear();

		std::string dungeonKey = std::to_string(dungeonID);
		if (!setJSON.contains(dungeonKey)) return;

		const auto& dungeonJson = setJSON.at(dungeonKey);
		if (!dungeonJson.contains("Floors") || !dungeonJson["Floors"].is_array()) return;

		for (const auto& floor : dungeonJson["Floors"]) {
			int floorID = floor.value("FloorID", 0);
			FloorData data;
			data.isFirst = true;
			data.enemySpawnCount = floor.value("EnemySpawnCount", 0);
  			data.bossSpawnCount = floor.value("BossSpawnCount", 0);
			data.treasureSpawnCount = floor.value("TreasureSpawnCount", 0);
			data.stairSpawnCount = floor.value("StairSpawnCount", 0);
			data.goalSpawnCount = floor.value("GoalSpawnCount", 0);

			dungeonFloorMap[floorID] = data;
		}
	}

public:
	/*
	 *	@brief		指定したIDのフロアデータの取得
	 *	@param[in]	int floorID				フロアID
	 *  @param[out] FloorData& floorData	取得したものを返すフロアデータ
	 *  @return		bool
	 */
	bool TryGetFloorData(int floorID, FloorData& floorData) {
		auto itr = dungeonFloorMap.find(floorID);
		if (itr == dungeonFloorMap.end()) return false;

		floorData = itr->second;
		return true;
	}
	/*
	 *	@brief		指定したIDのフロアデータのセット
	 *  @prram[in]	int floorID
	 *	@param[in]	FloorData setFloorData
	 *	@return		bool
	 */
	bool TrySetFloorData(int floorID, FloorData setFloorData) {
		auto itr = dungeonFloorMap.find(floorID);
		if (itr == dungeonFloorMap.end()) return false;

		itr->second = setFloorData;
		return true;
	}
	/*
	 *	@brief		最初の階層突入フラグ変更
	 *  @param[in]	int floorID
	 *  @param[in]	bool setFlag
	 */
	void MarkFloorEntered(int floorID, bool setFlag) {
		auto itr = dungeonFloorMap.find(floorID);
		if (itr == dungeonFloorMap.end()) return;

		itr->second.isFirst = setFlag;
	}
	/*
	 *	@brief		敵の生成数変更
	 *	@param[in]	int floorID
	 *	@param[in]	int setSpawnCount
	 */
	void SaveEnemySpawnCount(int floorID, int setSpawnCount) {
		auto itr = dungeonFloorMap.find(floorID);
		if (itr == dungeonFloorMap.end()) return;

		itr->second.enemySpawnCount = setSpawnCount;
	}
	/*
	 *	@brief		ボスの生成数変更
	 *	@param[in]	int floorID
	 *	@param[in]	int setSpawnCount
	 */
	void SaveBossSpawnCount(int floorID, int setSpawnCount) {
		auto itr = dungeonFloorMap.find(floorID);
		if (itr == dungeonFloorMap.end()) return;

		itr->second.bossSpawnCount = setSpawnCount;
	}
	/*
	 *	@brief		お宝の生成数変更
	 *	@param[in]	int floorID
	 *	@param[in]	int setSpawnCount
	 */
	void SaveTreasureSpawnCount(int floorID, int setSpawnCount) {
		auto itr = dungeonFloorMap.find(floorID);
		if (itr == dungeonFloorMap.end()) return;

		itr->second.treasureSpawnCount = setSpawnCount;
	}
};
#endif // !_DUNGEON_FLOOR_DATA_H_