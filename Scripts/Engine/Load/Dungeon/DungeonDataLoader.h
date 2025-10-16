/*
 *	@file	DungeonDataLoader.h
 *	@author Seki
 */

#ifndef _DUNGEON_DATA_LOADER_H_
#define _DUNGEON_DATA_LOADER_H_

#include "../CSV/LoadCSV.h"
#include "../../../Data/DungeonData.h"

#include <vector>
#include <string>

/*
 *  ダンジョン概要データ読み込みクラス
 */
class DungeonDataLoader : public LoadBase { // ← LoadBase を継承
private:
    LoadCSV csvLoader;

public:
    std::vector<DungeonData> dungeonList;

    DungeonDataLoader(const std::string& setFilePath)
        : LoadBase(setFilePath), csvLoader(setFilePath) {}

    void Load() override { // ← LoadBase の仮想関数をオーバーライド
        std::cout << "[DungeonDataLoader] Loading CSV: " << filePath << std::endl;
        csvLoader.Load();
        if (!csvLoader.IsLoaded()) {
            std::cout << "[DungeonDataLoader] CSV load failed!" << std::endl;
        }
        else {
            std::cout << "[DungeonDataLoader] CSV loaded successfully" << std::endl;
        }
        if (!csvLoader.IsLoaded()) return;

        const auto& csvData = csvLoader.GetData();

        // ヘッダ行をスキップして CSV を変換
        for (size_t i = 1; i < csvData.size(); ++i) {
            const auto& row = csvData[i];
            if (row.size() < 6) continue;

            DungeonData data;
            data.dungeonID = std::stoi(row[0]);
            data.name = row[1];
            data.openDay = std::stoi(row[2]);
            data.eventStartDay = std::stoi(row[3]);
            data.eventEndDay = std::stoi(row[4]);
            data.dungeonPath = row[5];

            dungeonList.push_back(data);
        }
    }

public:
    inline bool IsLoaded() const override {
        return csvLoader.IsLoaded();
    }
};

#endif // !_DUNGEON_DATA_LOADER_H_