/*
 *	@file	MoneyManager.h
 *	@author	Seki
 */

#ifndef _MONEY_MANAGER_H_
#define _MONEY_MANAGER_H_

#include "../../Singleton.h"
#include "../../../Data/TreasureDataList.h"
#include "../../VecMath.h"

#include <memory>
#include <string>

/*
 *	お金の管理クラス
 */
class MoneyManager : public Singleton<MoneyManager>{
	friend class Singleton<MoneyManager>;

private:
	std::unique_ptr<TreasureDataList> treasureDataList;		// お宝の価値データクラス
	int currentMoney = 0;									// 現在の所持金

private:
	/*
	 *	コンストラクタ
	 */
	MoneyManager() : treasureDataList(std::make_unique<TreasureDataList>()) {};
	/*
	 *	デストラクタ
	 */
	~MoneyManager() = default;

public:
	/*
	 *  JSONデータの読み込み、mapに自動で登録する
	 *  @param[in]  std::unordered_map<std::string, T>& map     登録対象のmap
	 */
	void LoadFromJSON(const JSON& setJSON) {
		treasureDataList->LoadFromJson(setJSON);
	}
public:
	/*
	 *	所持金の取得
	 *	@return		int
	 */
	inline int GetCurrentMoney() { return currentMoney; }
	/*
	 *	所持金の設定
	 *	@param[in]	int setValue	お金の値
	 */
	inline void SetCurrentMoney(int setValue) {
		currentMoney = setValue;
		if (currentMoney < 0) currentMoney = 0;
	}
	/*
	 *	所持金にお金を足す
	 *	@param[in]	const int ID	お宝のID
	 */
	inline void AddMoney(const int ID) {
		int addMoney;
		if (!treasureDataList->TryGetValue(ID, addMoney)) return;
		SetCurrentMoney(currentMoney + addMoney);
	}
	/*
	 *	所持金からお金を引く
	 *	@param[in]	int setValue	お金の値
	 */
	inline void SubMoney(int setValue) {
		SetCurrentMoney(currentMoney - setValue);
	}
};

#endif // !_MONEY_MANAGER_H_
