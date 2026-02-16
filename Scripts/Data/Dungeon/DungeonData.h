/*
 *	@file	DungeonData.h
 *	@author	Seki
 */

#ifndef _DUNGEON_DATA_H_
#define _DUNGEON_DATA_H_

#include <string>

/*
 *	@brief  ダンジョンの概要データ
 */
struct DungeonData {
    int dungeonID = 0;                  // ダンジョン識別ID（0はチュートリアル）
    std::string name = "";              // ダンジョン名
    int eventStartDay = 0;              // イベント開始日
    int eventEndDay = 0;                // イベント終了日
    int levelOfDanger = 0;              // 危険度レベル
    int necessaryStrength = 0;          // 必要なstrength
    int treasureCount = 0;              // お宝の数
    std::string dungeonPath = "";       // ダンジョンデータCSVやモデルパス
    bool isEventDay = false;            // イベントフラグ
};
/*
 *  @brief  ダンジョン情報
 */
struct DungeonInfoData {
    bool isUsed = false;
    bool isEventDay = false;
    int eventStartDay = 0;              // イベント開始日
    int eventEndDay = 0;                // イベント終了日
    int levelOfDanger = 0;              // 危険度レベル
    int necessaryStrength = 0;          // 必要なstrength
    int treasureCount = 0;              // お宝の数

};
#endif // !_DUNGEON_DATA_H_
