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
struct TreasureData {
	int ID;
	int value;
};

/*
 *	お宝の価値データクラス
 */
class TreasureDataList {
private:
	std::unordered_map<std::string, TreasureData> nameToData;	// 名前->IDのお宝データのマップ
	std::unordered_map<int, std::string> IDToName;				// ID->名前のお宝データのマップ

public:
	/*
	 *	コンストラクタ
	 */
	TreasureDataList() = default;

public:
	/*
	 *  JSONをmapに自動で登録する
	 *  @param[in]  const JSON& setJSON		読み込むJSONデータ
	 */
	void LoadFromJson(const JSON& setJSON) {
		for (auto& entry : setJSON.items()) {
			TreasureData data;
			data.ID = entry.value()["ID"].get<int>();
			data.value = entry.value()["Value"].get<int>();

			std::string name = entry.key();
			nameToData[name] = data;
			IDToName[data.ID] = name;
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
};

#endif // !_TREASURE_DATA_LIST_H_