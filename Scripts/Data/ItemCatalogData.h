/*
 *	@file	ItemCatalogData.h
 *	@author	Seki
 */

#ifndef _ITEM_CATALOG_DATA_H_
#define _ITEM_CATALOG_DATA_H_

#include "../Engine/JSON.h"

#include <unordered_map>
#include <string>

/*
 *	@brief	ItemData構造体
 */
struct ItemData {
	std::string name = "";	// アイテムの名前
	int price = 0;			// アイテムの値段
	int openDay = 0;		// アイテムの開放日
};

/*
 *	@brief	ショップに並ぶアイテムデータ
 */
class ItemCatalogData {
	std::unordered_map<int, std::shared_ptr<ItemData>> itemDataMap;		// アイテムデータマップ

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
	 *  @brief		JSONデータの読み込み、mapに自動で登録する
	 *  @param[in]  const JSON& setJSON                         読み込むJSONデータ
	 */
	void LoadFromJson(const JSON& setJSON) {
		for (auto& entry : setJSON.items()) {

			auto data = std::make_shared<ItemData>();

			int ID = std::stoi(entry.key());
			data->name = entry.value()["Name"].get<std::string>();
			data->price = entry.value()["Price"].get<int>();
			data->openDay = entry.value()["OpenDay"].get<int>();

			itemDataMap[ID] = data;
		}
	}

public:
	/*
	 *	@brief		IDからアイテムの値段を取得
	 *	@param[in]	const int itemID	アイテムのID
	 *	@param[out]	int& price			アイテムの値段
	 *	@return		bool
	 */
	bool TryGetPrice(const int itemID, int& price) {
		auto itr = itemDataMap.find(itemID);
		if (itr == itemDataMap.end()) return false;

		price = itr->second->price;
		return true;
	}
	/*
	 *	@brief		アイテムデータを全て取得
	 *  @param[out]	std::vector<ItemData>& itemDataList		アイテムデータ配列
	 *  @return		bool
	 */
	bool TryGetItemData(std::vector<ItemData*>& itemDataList) const {
		for (auto& itr : itemDataMap) {
			ItemData* item = itr.second.get();
			itemDataList.push_back(item);
		}
		if (itemDataList.empty())return false;

		return true;
	}
};
#endif // !_ITEM_CATALOG_DATA_H_
