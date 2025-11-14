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
 *	お宝の価値データクラス
 */
class TreasureDataList {
	std::unordered_map<std::string, int> treasureDataMap;	// お宝データのマップ

public:
	/*
	 *	コンストラクタ
	 */
	TreasureDataList() = default;

public:
    /*
     *  JSONデータの読み込み、mapに自動で登録する
	 *  @param[in]  std::unordered_map<std::string, T>& map     登録対象のmap
	 *  @param[in]  const JSON& setJSON                         読み込むJSONデータ
	 */
    void LoadFromJson(const JSON& setJSON) {
        // 自動登録クラス
        AutoJSONRegistry registry;
        // データの登録
        registry.LoadFromJson<int>(treasureDataMap, setJSON);
    }

public:
	/*
	 *  指定したお宝名の価値を取得
	 *  @param[in]  const std::string& treasureName 検索するお宝名
	 *  @param[out] int& value						お宝の価値
	 *  @return bool
	 */
	bool TryGetValue(const std::string& treasureName, int& value) const {
		auto it = treasureDataMap.find(treasureName);
		if (it == treasureDataMap.end()) return false;

		value = it->second;
		return true;
	}
};

#endif // !_TREASURE_DATA_LIST_H_
