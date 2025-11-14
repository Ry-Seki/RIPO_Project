/*
 *	@file	MoneyManager.h
 *	@author	Seki
 */

#ifndef _MONEY_MANAGER_H_
#define _MONEY_MANAGER_H_

#include "../../Singleton.h"
#include "../../VecMath.h"

/*
 *	お金の管理クラス
 */
class MoneyManager : public Singleton<MoneyManager>{
	friend class Singleton<MoneyManager>;

private:
	int currentMoney = 0;	// 現在の所持金

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
	 *	@param[in]	int setValue	お金の値
	 */
	inline void AddMoney(int setValue) {
		SetCurrentMoney(currentMoney + setValue);
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
