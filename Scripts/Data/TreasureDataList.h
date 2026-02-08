/*
 *	@file	TreasureDataList.h
 *	@author	Seki
 */

#ifndef _TREASURE_DATA_LIST_H_
#define _TREASURE_DATA_LIST_H_

#include "../Engine/JSON.h"
#include "../Engine/AutoJSONRegitry.h"

#include <unordered_map>
#include <string>

 /*
  *	お宝構造体
  */
struct TreasureValueData {
	int ID;
	int value;
};

/*
 *	お宝のステータス構造体
 */
struct TreasureStatusData {
	int ID;
	std::string effectName;
	int strength;
};

/*
 *	お宝の価値データクラス
 */
class TreasureValueDataList {
private:
	std::unordered_map<std::string, TreasureValueData> nameToData;	// 名前->IDのお宝データのマップ
	std::unordered_map<int, std::string> IDToName;					// ID->名前のお宝データのマップ

public:
	static std::unordered_map<int, TreasureStatusData >statusIDToData;
public:
	/*
	 *	コンストラクタ
	 */
	TreasureValueDataList() = default;

public:
	/*
	 *  JSONをmapに自動で登録する
	 *  @param[in]  const JSON& setJSON		読み込むJSONデータ
	 */
	void LoadFromJson(const JSON& setJSON) {
		for (auto& entry : setJSON.items()) {
			TreasureValueData data;
			data.ID = entry.value()["ID"].get<int>();
			data.value = entry.value()["Value"].get<int>();

			std::string name = entry.key();
			nameToData[name] = data;
			IDToName[data.ID] = name;

			TreasureStatusData statusData;
			statusData.ID = entry.value()["ID"].get<int>();
			statusData.effectName = entry.value()["Effect"].get < std::string >();
			statusData.strength = entry.value()["Strength"].get<int>();

			statusIDToData[statusData.ID] = statusData;
		}
	}

public:
	/*
	 *	指定したIDからNameを取得
	 *	@param[in]	const int ID
	 *	@param[out]	std::string& treasureName
	 *  @return		bool
	 */
	bool TryGetName(const int ID, std::string& treasureName) {
		auto itr = IDToName.find(ID);
		if (itr == IDToName.end()) return false;

		treasureName = itr->second;
		return true;
	}
	/*
	 *	指定したNameからIDを取得
	 *	@param[in]	const std::string& treasureName
	 *	@param[out]	int& ID
	 */
	bool TryGetID(const std::string& treasureName, int& ID) {
		auto itr = nameToData.find(treasureName);
		if (itr == nameToData.end()) return false;

		ID = itr->second.ID;
		return true;
	}
	/*
	 *  指定したNameからvalueを取得
	 *  @param[in]  const std::string& treasureName 検索するお宝名
	 *  @param[out] int& value						お宝の価値
	 *  @return bool
	 */
	bool TryGetValue(const std::string& treasureName, int& value) const {
		auto itr = nameToData.find(treasureName);
		if (itr == nameToData.end()) return false;

		value = itr->second.value;
		return true;
	}
	/*
	 *  指定したIDからvalueを取得
	 *  @param[in]  const std::string& treasureName 検索するお宝名
	 *  @param[out] int& value						お宝の価値
	 *  @return bool
	 */
	bool TryGetValue(const int ID, int& value) const {
		// まず ID から名前を取得
		auto itrName = IDToName.find(ID);
		if (itrName == IDToName.end()) return false;

		// 次に名前から TreasureData を取得
		auto itrData = nameToData.find(itrName->second);
		if (itrData == nameToData.end()) return false;

		// value を返す
		value = itrData->second.value;
		return true;
	}

	/*
	 *  指定したIDからお宝のステータスを取得する
	 *  @param[in]  const int ID			検索するお宝のID
	 *  @param[out] TreasureStatusData& data
	 *  @return bool
	 *  @author oorui
	 */
	static bool TryGetTreasureStatus(const int ID, TreasureStatusData& data) {
		// IDからステータスを取得
		auto itr = statusIDToData.find(ID);
		if (itr == statusIDToData.end()) return false;
		// ステータスを取得
		data = itr->second;

		return true;
	}

	/*
	 *  IDからエフェクトの名前を取得
	 *  @param[in]  const int ID			検索するお宝の名前
	 *  @param[out] std::string& effect		エフェクトの名前の文字列
	 *  @return bool
	 *  @author oorui
	 */
	static bool TryGetEffectName(const int ID, std::string& effect) {
		// IDからステータスを取得
		auto itr = statusIDToData.find(ID);
		if (itr == statusIDToData.end()) return false;
		// エフェクトの名前を取得、返す
		effect = itr->second.effectName;

		return true;
	}

	/*
	 *  IDからお宝の筋力値を取得
	 *  @param[in]  const int ID			検索するお宝の名前
	 *  @param[out] std::string& effect		金慮p口
	 *  @return bool
	 *  @author oorui
	 */
	static bool TryGetStrength(const int ID, int& strength) {
		// IDからステータスを取得
		auto itr = statusIDToData.find(ID);
		if (itr == statusIDToData.end()) return false;
		// 筋力値を取得
		strength = itr->second.strength;

		return true;
	}
};

#endif // !_TREASURE_DATA_LIST_H_