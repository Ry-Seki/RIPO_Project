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
    int minStrength = 0;                // 最小strength
    int maxStrength = 0;                // 最大strength
    int treasureCount = 0;              // お宝の数
    std::string dungeonPath = "";       // ダンジョンデータCSVやモデルパス
    bool isEventDay = false;            // イベントフラグ
};
/*
 *  @brief  ダンジョン情報
 */
struct DungeonInfoData {
    bool isEventClear = false;    // イベントお宝回収フラグ
    bool isEventDay = false;      // イベント日フラグ
    int eventStartDay = 0;        // イベント開始日
    int eventEndDay = 0;          // イベント終了日
    int levelOfDanger = 0;        // 危険度レベル
    int minStrength = 0;          // 最小strength
    int maxStrength = 0;          // 最大Strength
    int treasureCount = 0;        // お宝の数(イベントお宝は含めない)
    int maxTreasureCount = 0;     // ダンジョンの最大お宝の数(イベントお宝は含めない)

};
#endif // !_DUNGEON_DATA_H_
