/*
 *	@file	SaveData.h
 *	@author	Seki
 */

#ifndef _SAVE_DATA_H_
#define _SAVE_DATA_H_

/*
 *	@brief	セーブデータ構造体(可視部分)
 */
struct UserData {
	int playTime;		// 累計プレイ時間
	int elapsedDay;		// 経過日数
	bool isHalfDay;		// 半日消費フラグ
	int currentMoney;	// 現在の所持金
	int totalTreasureCount;	// イベントお宝回収、ボス突破フラグ(ビットで管理)
};
#endif // !_SAVE_DATA_H_