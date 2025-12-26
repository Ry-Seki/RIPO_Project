/*
 *	@file	SaveData.h
 *	@author	Seki
 */

#ifndef _SAVE_DATA_H_
#define _SAVE_DATA_H_

/*
 *	セーブデータ構造体
 */
struct UserData {
	int elapsedDay;		// 経過日数
	bool isHalf;		// 半日フラグ
	int playTime;		// 累計プレイ時間
	int currentMoney;	// 現在の所持金
	int dungeonFlags;	// イベントお宝回収、ボス突破フラグ(ビットで管理)
};
#endif // !_SAVE_DATA_H_