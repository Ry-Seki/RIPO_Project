/*
 *	@file	SaveData.h
 *	@author	Seki
 */

#ifndef _SAVE_DATA_H_
#define _SAVE_DATA_H_

#include <unordered_map>

// 前方宣言
struct DungeonProgressData;

/*
 *	@brief	プレイヤー進行データ
 */
struct GameProgressData {
	int playTime;		// 累計プレイ時間
	int elapsedDay;		// 経過日数
	bool isHalfDay;		// 半日消費フラグ
	int currentMoney;	// 現在の所持金
	int totalTreasureCount;	// イベントお宝回収、ボス突破フラグ(ビットで管理)
};
/*
 *	@breif	プレイヤーステータスレベルデータ
 */
struct PlayerProgressData {
	int hpLevel;
	int staminaLevel;
	int strengthLevel;
	int resistTimeLevel;
};
/*
 *	@brief	ワールド進行データ
 */
struct WorldProgressData {
	// dungeonID -> ダンジョン進行
	std::unordered_map<int, DungeonProgressData> dungeonProgress;

	// グローバルなお宝ID管理（累計管理用）
	std::vector<int> obtainedTreasureIDs;

};
#endif // !_SAVE_DATA_H_