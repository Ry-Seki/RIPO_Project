/*
 *	@file	DungeonEntranceData.h
 *  @author oorui
 */

#ifndef _DUNGEONENTRANCEDATA_H_
#define _DUNGEONENTRANCEDATA_H_

#include "../../Engine/JSON.h"
#include "../../Engine/VecMath.h"
#include "../../Engine/GameConst.h"

#include <unordered_map>
#include <vector>
#include <string>

 /*
  *	@brief	出口、階段データ
  */
struct EntranceData {
	int moveID = 0;
	std::string name = "";
	Vector3 position = V_ZERO;
	Vector3 rotation = V_ZERO;
	Vector3 AABBmin = V_ZERO;
	Vector3 AABBmax = V_ZERO;
};

class DungeonEntranceData {
private:
	std::unordered_map<int, std::vector<EntranceData>> dungeonEntranceMap;

public:
	/*
	 *	@brief	コンストラクタ
	 */
	DungeonEntranceData() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~DungeonEntranceData() = default;

public:
	/*
	 *	@brief		データのリセット処理
	 */
	void ClearEntranceData() {
		dungeonEntranceMap.clear();
	}
	/*
	 *  @brief      JSONデータの読み込み、mapに自動で登録する
	 *  @param[in]  const JSON& setJSON     読み込むJSONデータ
	 */
	void LoadFromJSON(const JSON& setJSON) {
		dungeonEntranceMap.clear();

		// Floor配列を取得
		const auto& floors = setJSON["Floor"];
		if (!floors.is_array()) return;

		// Floorごとに処理
		for (size_t floorIndex = 0; floorIndex < floors.size(); ++floorIndex) {
			LoadEntranceArray(floorIndex, floors[floorIndex], GameConst::_CREATE_POSNAME_STAIR);	// 階段
			LoadEntranceArray(floorIndex, floors[floorIndex], GameConst::_CREATE_POSNAME_GOAL);		// 出口
		}
	}
	/*
	 *	@brief
	 */
	void LoadEntranceArray(int floorID, const JSON& floorJson, const std::string& key) {
		if (!floorJson.contains(key) || !floorJson[key].is_array()) {
			return;
		}
		const auto& entranceArray = floorJson[key];

		// Entrance配列を走査
		for (size_t i = 0; i < entranceArray.size(); ++i) {
			EntranceData data;

			// MoveID
			data.moveID = entranceArray[i].value("MoveID", 0);

			// 名前を取得
			data.name = entranceArray[i].value("name", "");

			// positionを取得
			if (entranceArray[i].contains("position") && entranceArray[i]["position"].is_array()) {
				data.position.x = entranceArray[i]["position"][0];
				data.position.y = entranceArray[i]["position"][1];
				data.position.z = entranceArray[i]["position"][2];
			}

			// rotationを取得
			if (entranceArray[i].contains("rotation") && entranceArray[i]["rotation"].is_array()) {
				data.rotation.x = entranceArray[i]["rotation"][0];
				data.rotation.y = entranceArray[i]["rotation"][1];
				data.rotation.z = entranceArray[i]["rotation"][2];
			}

			// AABB最小値を取得
			if (entranceArray[i].contains("AABBmin") && entranceArray[i]["AABBmin"].is_array()) {
				data.AABBmin.x = entranceArray[i]["AABBmin"][0];
				data.AABBmin.y = entranceArray[i]["AABBmin"][1];
				data.AABBmin.z = entranceArray[i]["AABBmin"][2];
			}

			// AABB最大値を取得
			if (entranceArray[i].contains("AABBmax") && entranceArray[i]["AABBmax"].is_array()) {
				data.AABBmax.x = entranceArray[i]["AABBmax"][0];
				data.AABBmax.y = entranceArray[i]["AABBmax"][1];
				data.AABBmax.z = entranceArray[i]["AABBmax"][2];
			}
			dungeonEntranceMap[floorID].push_back(data);
		}
	}
	/*
	 *	@brief		フロアID指定の出口、階段データの取得
	 *	@param[in]	int floorID
	 *	@param[out]	std::vector<EntranceData>& data
	 *	@return		bool
	 */
	bool TryGetEntranceData(int floorID, std::vector<EntranceData>& data) {
		auto itr = dungeonEntranceMap.find(floorID);
		if (itr == dungeonEntranceMap.end()) return false;

		data = itr->second;
		return true;
	}
};

#endif // !_DUNGEONENTRANCEDATA_H_
