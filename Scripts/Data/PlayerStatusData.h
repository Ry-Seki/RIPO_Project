/*
 *	@file	PlayerStatusData.h
 *	@author	Seki
 */

#ifndef _PLAYER_STATUS_DATA_H_
#define _PLAYER_STATUS_DATA_H_

/*
 *	プレイヤーのステータス値
 */
struct PlayerStatusValue {
	int HP;
	int stamina;
	int strength;
	int resistTime;

	/*
	 *	@brief	構造体を配列のようにするためのオペレーター
	 */
	int& operator[](int index) {
		switch (index) {
			case 0: return HP;
			case 1: return stamina;
			case 2: return strength;
			case 3: return resistTime;
		}
	}
	/*
	 *	@brief	構造体を配列のようにするためのオペレーター
	 */
	const int& operator[](int index) const {
		switch (index) {
			case 0: return HP;
			case 1: return stamina;
			case 2: return strength;
			case 3: return resistTime;
		}
	}

	static constexpr int Length() { return 4; }
};
/*
 *	プレイヤーのステータスデータ
 */
struct PlayerStatusData {
	PlayerStatusValue base;
	PlayerStatusValue rise;
	PlayerStatusValue lv;
};

#endif // !_PLAYER_STATUS_DATA_H_