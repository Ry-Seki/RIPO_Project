/*
 *	@file	DungeonDataLoader.h
 *	@author Seki
 */

#ifndef _DUNGEON_DATA_LOADER_H_
#define _DUNGEON_DATA_LOADER_H_

#include "../CSV/LoadCSV.h"
#include "../../../Data/Dungeon/DungeonData.h"

#include <vector>
#include <string>
#include <cassert>

/*
 *  @brief  ダンジョン概要データ読み込みクラス
 */
class DungeonDataLoader : public LoadBase { // ← LoadBase を継承
private:
    LoadCSV csvLoader;

public:
    std::vector<DungeonData> dungeonList;

public:
    /*
     *  @brief  コンストラクタ
     */
    DungeonDataLoader(const std::string& setFilePath)
        : LoadBase(setFilePath), csvLoader(setFilePath) {}

public:
    /*
     *  @brief  ロード処理
     */
    void Load() override { 
        csvLoader.Load();
        if (!csvLoader.IsLoaded()) {
            assert(false && "DungeonData.csvが読み込めませんでした");
            return;
        }
        const auto& csvData = csvLoader.GetData();

        // ヘッダ行をスキップしてCSVを変換
        for (size_t i = 1; i < csvData.size(); ++i) {
            const auto& row = csvData[i];
            if (row.size() < 9) continue;

            DungeonData data;
            data.dungeonID = std::stoi(row[0]);
            data.name = row[1];
            data.eventStartDay = std::stoi(row[2]);
            data.eventEndDay = std::stoi(row[3]);
            data.levelOfDanger = std::stoi(row[4]);
            data.minStrength = std::stoi(row[5]);
            data.maxStrength = std::stoi(row[6]);
            data.treasureCount = std::stoi(row[7]);
            data.dungeonPath = row[8];

            dungeonList.push_back(data);
        }
    }

public:
    /*
     *  @brief      ロード状況の取得
     *  @return     bool
     */
    inline bool IsLoaded() const override {
        return csvLoader.IsLoaded();
    }
};

#endif // !_DUNGEON_DATA_LOADER_H_