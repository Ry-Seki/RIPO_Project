/*
 *	@file	DungeonData.h
 *	@author	Seki
 */

#ifndef _DUNGEON_DATA_H_
#define _DUNGEON_DATA_H_

#include <string>

/*
 *	ダンジョンの概要データ
 */
struct DungeonData {
    int dungeonID;              // ダンジョン識別ID（0始まり）
    std::string name;           // ダンジョン名
    int openDay;                // ダンジョン開放日（0始まり）
    int eventStartDay;          // イベント開始日
    int eventEndDay;            // イベント終了日
    bool isEventTreasureGet;    // お宝獲得フラグ
    bool isBossDefeated;        // ボス突破フラグ
    std::string dungeonPath;    // ダンジョンデータCSVやモデルパス
};
#endif // !_DUNGEON_DATA_H_
