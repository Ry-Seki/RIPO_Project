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
	
	static constexpr const char* _FLOOR = "Floor";
	static constexpr const char* _POSITION = "position";
	static constexpr const char* _ROTATION = "rotation";
	static constexpr const char* _MIN = "AABBmin";
	static constexpr const char* _MAX = "AABBmax";
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
		const auto& floors = setJSON[_FLOOR];
		if (!floors.is_array()) return;

		// Floorごとに処理
		for (size_t floorIndex = 0; floorIndex < floors.size(); ++floorIndex) {
			LoadEntranceArray(floorIndex, floors[floorIndex], GameConst::_CREATE_POSNAME_STAIR);	// 階段
			LoadEntranceArray(floorIndex, floors[floorIndex], GameConst::_CREATE_POSNAME_GOAL);		// 出口
		}
	}
	/*
	 *	@brief	階段と出口のデータを取得
	 *  @param[in]	floorID		階層のID
	 *  @param[in]	floorjson	階層のJSON
	 *  @param[in]	key			識別
	 */
	void LoadEntranceArray(int floorID, const JSON& floorJson, const std::string& key) {
		if (!floorJson.contains(key) || !floorJson[key].is_array()) {
			return;
		}
		const auto& entranceArray = floorJson[key];

		// Entrance配列を走査
		for (size_t i = 0; i < entranceArray.size(); i++) {
			EntranceData data;

			// MoveID
			data.moveID = entranceArray[i].value("MoveID", 0);

			// 名前を取得
			data.name = entranceArray[i].value("name", "");

			// positionを取得
			if (entranceArray[i].contains(_POSITION) && entranceArray[i][_POSITION].is_array()) {
				data.position.x = entranceArray[i][_POSITION][0];
				data.position.y = entranceArray[i][_POSITION][1];
				data.position.z = entranceArray[i][_POSITION][2];
			}

			// rotationを取得
			if (entranceArray[i].contains(_ROTATION) && entranceArray[i][_ROTATION].is_array()) {
				data.rotation.x = entranceArray[i][_ROTATION][0];
				data.rotation.y = entranceArray[i][_ROTATION][1];
				data.rotation.z = entranceArray[i][_ROTATION][2];
			}

			// AABB最小値を取得
			if (entranceArray[i].contains(_MIN) && entranceArray[i][_MIN].is_array()) {
				data.AABBmin.x = entranceArray[i][_MIN][0];
				data.AABBmin.y = entranceArray[i][_MIN][1];
				data.AABBmin.z = entranceArray[i][_MIN][2];
			}

			// AABB最大値を取得
			if (entranceArray[i].contains(_MAX) && entranceArray[i][_MAX].is_array()) {
				data.AABBmax.x = entranceArray[i][_MAX][0];
				data.AABBmax.y = entranceArray[i][_MAX][1];
				data.AABBmax.z = entranceArray[i][_MAX][2];
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
