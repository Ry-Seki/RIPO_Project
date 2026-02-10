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
	int itenID = -1;
	int category = -1;		// アイテムのカテゴリー
	std::string name = "";	// アイテムの名前
	int price = -1;			// アイテムの値段
	bool buyCount = false;	// 購入上限
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
		for (auto& categoryEntry : setJSON.items()) {
			int categoryID = std::stoi(categoryEntry.key());

			for (auto& itemEntry : categoryEntry.value().items()) {
				auto data = std::make_shared<ItemData>();
				int itemID = std::stoi(itemEntry.key());

				data->itenID = itemID;
				data->category = categoryID;
				data->name = itemEntry.value()["Name"].get<std::string>();
				data->price = itemEntry.value()["Price"].get<int>();
				data->buyCount = itemEntry.value()["BuyCount"].get<bool>();
				itemDataMap[itemID] = data;
			}
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
	 *	@brief		IDからアイテムデータの取得
	 *	@param[in]	int itemID
	 *	@param[out]	const ItemData*& outItem
	 *	@return		bool
	 */
	bool TryGetItem(int itemID, ItemData*& outItem) const {
		auto itr = itemDataMap.find(itemID);
		if (itr == itemDataMap.end()) return false;

		outItem = itr->second.get();
		return true;
	}
	/*
	 *	@brief		アイテムデータを全て取得
	 *  @param[out]	std::vector<ItemData>& itemDataList		アイテムデータ配列
	 *  @return		bool
	 */
	bool TryGetAllItemsData(std::vector<ItemData*>& itemDataList) const {
		for (auto& itr : itemDataMap) {
			ItemData* item = itr.second.get();
			itemDataList.push_back(item);
		}
		if (itemDataList.empty())return false;

		return true;
	}
};
#endif // !_ITEM_CATALOG_DATA_H_
