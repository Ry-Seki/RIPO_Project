/*
 *	@file	ItemCatalogData.h
 *	@author	Seki
 */

#ifndef _ITEM_CATALOG_DATA_H_
#define _ITEM_CATALOG_DATA_H_

#include "../Engine/AutoJSONRegitry.h"

#include <unordered_map>
#include <string>

/*
 *	ショップに並ぶアイテムデータ
 */
class ItemCatalogData {
	std::unordered_map<std::string, int> itemDataMap;		// アイテムデータマップ

public:
	/*
	 *	コンストラクタ
	 */
	ItemCatalogData() = default;
	/*
	 *	デストラクタ
	 */
	~ItemCatalogData() = default;

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
		registry.LoadFromJson<int>(itemDataMap, setJSON);
	}

public:
	/*
	 *	名前からアイテムの値段を取得
	 *	@param[in]	const std::string& itemName		アイテムの名前
	 *	@param[out]	int& value						アイテムの値段
	 *  @return		bool
	 */
	bool TryGetValue(const std::string& treasureName, int& value) const {
		auto it = itemDataMap.find(treasureName);
		if (it == itemDataMap.end()) return false;

		value = it->second;
		return true;
	}
};
#endif // !_ITEM_CATALOG_DATA_H_
