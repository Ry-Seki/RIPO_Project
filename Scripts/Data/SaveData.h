/*
 *	@file	SaveData.h
 *	@author	Seki
 */

#ifndef _SAVE_DATA_H_
#define _SAVE_DATA_H_

#include "Dungeon/DungeonProgressData.h"

#include <unordered_map>

/*
 *	@brief	ゲーム進行データ
 */
struct GameProgressData {
	int playTime;			// 累計プレイ時間
	int elapsedDay;			// 経過日数
	bool isHalfDay;			// 半日消費フラグ
	int currentMoney;		// 現在の所持金
	int totalTreasureCount;	// 累計お宝獲得数
};
/*
 *	@breif	プレイヤーステータスレベルデータ
 */
struct PlayerStatusLevelData {
	int hpLevel;
	int staminaLevel;
	int strengthLevel;
	int resistTimeLevel;
};
/*
 *	@brief	ワールド進行データ
 */
struct WorldProgressData {
	std::unordered_map<int, DungeonProgressData> dungeonProgress;	// dungeonID -> ダンジョン進行
	std::vector<int> getTreasureIDList;								// お宝ID管理（累計管理用）
};
/*
 *	@brief	設定データ
 */
struct SettingsData {
	float mouseSensitivity;
	float masterVolume;
	float bgmVolume;
	float seVolume;
};
/*
 *	@brief	セーブデータ
 */
struct SaveData {
	bool isUsed = false;
	bool isClear = false;
	GameProgressData game;
	PlayerStatusLevelData player;
	WorldProgressData world;
};

#endif // !_SAVE_DATA_H_