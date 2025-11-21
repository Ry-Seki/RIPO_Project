/*
 *  @file   SokobanMapManager.cpp
 *  @author Seki
 */

#include "SokobanMapManager.h"

/*
 *  @brief      フォルダの読み込み
 *  @param[in]  const std::string& folderPath    読み込み対象のフォルダー
 */
bool SokobanMapManager::LoadMapList(const std::string& folderPath) {
    mapFileList.clear();

    try {
        if (!std::filesystem::exists(folderPath)) return false;
        if (!std::filesystem::is_directory(folderPath)) return false;

        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (!entry.is_regular_file()) continue;

            if (entry.path().extension() == ".txt") {
                mapFileList.push_back(entry.path().string());
            }
        }
    }catch (const std::exception& exception) {
        return false;
    }

    isLoaded = !mapFileList.empty();
    return isLoaded;
}

/*
 *  @brief  ランダムなマップファイル１つを返す
 */
std::string SokobanMapManager::GetRandomMap() const {
    if (!isLoaded || mapFileList.empty()) return "";

    static std::random_device rd;
    static std::mt19937 mt(rd());

    std::uniform_int_distribution<int> dist(0, (int)mapFileList.size() - 1);

    return mapFileList[dist(mt)];
}