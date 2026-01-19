/*
 *	@file	SokobanMapCreater.h
 *	@author	Seki
 */

#ifndef _SOKOBAN_MAP_CTRATER_H_
#define _SOKOBAN_MAP_CTRATER_H_

#include <vector>
#include <string>
#include <fstream>

/*
 *	@brief	タイルの種類
 */
enum class TileType {
    Empty,
    Wall,
    Box,
    Goal,
    Player
};


/*
 *	@brief	倉庫番ミニゲームマップ生成クラス
 */
class SokobanMapCreater {
private:
    std::vector<std::vector<TileType>> map;

public:
    /*
     *  @brief      マップ生成(テキストファイルから出力)
     *  @param[in]  const std::string& setFilePath
     */
    bool LoadMapFromFile(const std::string& setfilePath) {
        std::ifstream file(setfilePath);
        if (!file.is_open()) return false;

        map.clear();
        std::string line;
        while (std::getline(file, line)) {
            std::vector<TileType> row;
            for (char c : line) {
                TileType tile;
                switch (c) {
                    case '#': tile = TileType::Wall; break;
                    case 'P': tile = TileType::Player; break;
                    case '@': tile = TileType::Box; break;
                    case 'G': tile = TileType::Goal; break;
                    default:  tile = TileType::Empty; break;
                }
                row.push_back(tile);
            }
            map.push_back(row);
        }
        file.close();
        return true;
    }

public:
    /*
     *  @brief      マップ取得
     *  @return     std::vector<std::vector<TileType>>&
     */
    inline const std::vector<std::vector<TileType>>& GetMap() const { return map; }
};

#endif // !_SOKOBAN_MAP_CTRATER_H_