/*
 *	@file	DungeonStageData.h
 *	@author Seki
 */

#ifndef _DUNGEON_STAGE_DATA_H_
#define _DUNGEON_STAGE_DATA_H_

#include <string>
#include <vector>

/* 
 *  ダンジョンステージデータ
 */
struct DungeonStageData {
    int stageID = -1;
    std::string name;
    std::string tilesetPath;
    std::string musicPath;
    std::vector<std::string> modelPaths;

    // 拡張用フィールド（イベント、宝箱、BGMなど）を追加可能
};
#endif // !_DUNGEON_STAGE_DATA_H_
