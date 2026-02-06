/*
 *	@file	MoneyManager.h
 *	@author	Seki
 */

#ifndef _MONEY_MANAGER_H_
#define _MONEY_MANAGER_H_

#include "../../Singleton.h"
#include "../../../Data/TreasureDataList.h"
#include "../../../Data/ItemCatalogData.h"

#include "../../VecMath.h"

#include <memory>
#include <string>

/*
 *	お金の管理クラス
 */
class MoneyManager : public Singleton<MoneyManager>{
	friend class Singleton<MoneyManager>;

private:
	TreasureValueDataList treasureDataList;	// お宝の価値データクラス
	ItemCatalogData itemCatalogData;	// アイテムカタログデータ

	int currentMoney = 0;				// 現在の所持金
	int prevMoney = 0;					// ひとつ前の所持金
	int highScoreMoney = 0;				// ハイスコアのお金 TODO : ハイスコア時の処理を行う

private:
	/*
	 *	コンストラクタ
	 */
	MoneyManager() = default;
	/*
	 *	デストラクタ
	 */
	~MoneyManager() = default;

public:
	/*
	 *  @brief		SONデータの読み込み、mapに自動で登録する
	 *  @param[in]  const JSON& setTreasureJSON
	 *  @param[in]	const JSON& setItemJSON
	 */
	void LoadFromJSON(const JSON& setTreasureJSON, const JSON& setItemJSON) {
		treasureDataList.LoadFromJson(setTreasureJSON);
		itemCatalogData.LoadFromJson(setItemJSON);
	}
	/*
	 *	@brief		アイテムデータのポインタを貸し出す
	 *	@param[out]	std::vector<ItemData>& itemDataList
	 */
	void SetItemDataList(std::vector<ItemData*>& itemDataList) {
		itemCatalogData.TryGetAllItemsData(itemDataList);
	}
	/*
	 *	@brief		アイテムカタログデータの取得
	 *	@return		ItemCatalogData
	 */
	ItemCatalogData GetItemCatalogData() const { return itemCatalogData; }

public:
	/*
	 *	@brief		現在の所持金の取得
	 *	@return		int
	 */
	inline int GetCurrentMoney() { return currentMoney; }
	/*
	 *	@brief		ひとつ前の所持金の取得
	 *	@return		int
	 */
	inline int GetPrevMoney() { return prevMoney; }
	/*
	 *	@brief		所持金の設定
	 *	@param[in]	int setValue	お金の値
	 */
	inline void SetMoney(int setValue) {
		prevMoney = currentMoney;
		currentMoney = setValue;
		if (currentMoney < 0) currentMoney = 0;
	}
	/*
	 *	@brief		所持金に足す
	 *	@param[in]	const int setValue	お金の値
	 */
	inline void AddMoney(const int setValue) {
		SetMoney(currentMoney + setValue);
	}
	/*
	 *	@brief		お宝の価値を所持金に足す
	 *	@param[in]	const int ID	お宝ID
	 */
	inline void AddTreasureMoney(const int ID) {
		int addMoney;
		if (!treasureDataList.TryGetValue(ID, addMoney)) return;
		AddMoney(addMoney);
	}
	/*
	 *	@brief		所持金から減らす
	 *	@param[in]	const int setValue	お金の値
	 */
	inline void SubMoney(const int setValue) {
		SetMoney(currentMoney - setValue);
	}
	/*
	 *	@brief		所持金からお金を引く
	 *	@param[in]	const int ID	アイテムID
	 */
	inline void SubItemMoney(const int ID) {
		int subMoney;
		if (!itemCatalogData.TryGetPrice(ID, subMoney)) return;
		SubMoney(subMoney);
	}
};

#endif // !_MONEY_MANAGER_H_