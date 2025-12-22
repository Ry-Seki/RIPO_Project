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
    int dungeonID = 0;                  // ダンジョン識別ID（0はチュートリアル）
    std::string name = "";              // ダンジョン名
    int openDay = 0;                    // ダンジョン開放日（0はチュートリアル）
    int eventStartDay = 0;              // イベント開始日
    int eventEndDay = 0;                // イベント終了日
    bool isEventTreasureGet = false;    // お宝獲得フラグ
    bool isBossDefeated = false;        // ボス突破フラグ
    std::string dungeonPath = "";       // ダンジョンデータCSVやモデルパス
};
#endif // !_DUNGEON_DATA_H_
