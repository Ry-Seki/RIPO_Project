/*
 *	@file	PlayerStatusData.h
 *	@author	Seki
 */

#ifndef _PLAYER_STATUS_DATA_H_
#define _PLAYER_STATUS_DATA_H_

#include "../Engine/GameEnum.h";

/*
 *	@brief	プレイヤーのステータス値
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
	/*
	 *	@brief	構造体のサイズの取得
	 *	@return	int
	 */
	 int Length() { return static_cast<int>(GameEnum::PlayerStatusType::Max); }
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