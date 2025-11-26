/*
 *	@file	DungeonFloorData.h
 *	@author	Seki
 */

#ifndef _DUNGEON_FLOOR_DATA_H_
#define _DUNGEON_FLOOR_DATA_H_

#include "../Engine/JSON.h"

#include <unordered_map>
#include <vector>

/*
 *	@brief	ダンジョン階層データ
 */
struct FloorData {
	int enemySpawnCount = 0;
	int bossSpawnCount = 0;
	int treasureSpawnCount = 0;
	int stairSpawnCount = 0;
	int goalSpawnCount = 0;
};

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
	 */
	void LoadFromJSON(const JSON& setJSON, int dungeonID) {
		dungeonFloorMap.clear();

		std::string dungeonKey = std::to_string(dungeonID);
		if (!setJSON.contains(dungeonKey)) return;

		const auto& dungeonJson = setJSON.at(dungeonKey);
		const auto& floors = dungeonJson["Floors"];

		for (const auto& floor : floors) {
			int floorID = floor.value("FloorID", 0);
			FloorData data;
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
	 *	@param[in]	const int floorID
	 *  @param[out] FloorData floorData
	 *  @return		bool
	 */
	bool TryGetFloorData(const int floorID, FloorData floorData) {
		auto itr = dungeonFloorMap.find(floorID);
		if (itr == dungeonFloorMap.end()) return false;

		floorData = itr->second;
		return true;
	}
	
};
#endif // !_DUNGEON_FLOOR_DATA_H_