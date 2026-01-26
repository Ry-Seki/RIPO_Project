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
    int eventStartDay = 0;              // イベント開始日
    int eventEndDay = 0;                // イベント終了日
    int levelOfDanger = 0;              // 危険度レベル
    int necessaryStrength = 0;          // 必要なstrength
    std::string dungeonPath = "";       // ダンジョンデータCSVやモデルパス
    bool isEventDay = false;            // イベントフラグ
};
#endif // !_DUNGEON_DATA_H_
