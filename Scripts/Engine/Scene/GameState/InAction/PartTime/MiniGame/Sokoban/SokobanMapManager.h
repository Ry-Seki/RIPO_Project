/*
 *	@file	SokobanMapManager.h
 *	@author	Seki
 */

#ifndef _SOKOBAN_MAP_MANAGER_H_
#define _SOKOBAN_MAP_MANAGER_H_

#include <vector>
#include <string>
#include <filesystem>

/*
 *	@brief	倉庫番のマップをフォルダーから持ってくるクラス
 */
class SokobanMapManager {
private:
    std::vector<std::string> mapFileList;
    bool isLoaded = false;

public:
    /*
     *  @brief  コンストラクタ
     */
    SokobanMapManager() = default;
    /*
     *  @brief  デストラクタ
     */
    ~SokobanMapManager() = default;

public:
    /*
     *  @brief      フォルダの読み込み
     *  @param[in]  const std::string& folderPath    読み込み対象のフォルダー
     */
    bool LoadMapList(const std::string& folderPath);

public:
    /*
     *  @brief      ランダムなマップの取得
     *  @return     std::string
     */
    std::string GetRandomMap() const;
    /*
     *  @brief      マップリストの取得
     *  @return     const std::vector<std::string>&
     */
    inline const std::vector<std::string>& GetMapList() const { return mapFileList; }
    /*
     *  @brief      ロード中フラグの取得
     *  @return     bool
     */
    inline bool IsLoaded() const { return isLoaded; }
};

#endif // !_SOKOBAN_MAP_MANAGER_H_