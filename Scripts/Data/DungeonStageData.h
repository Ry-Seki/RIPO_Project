/*
 *	@file	DungeonStageData.h
 *	@author Seki
 */

#ifndef _DUNGEON_STAGE_DATA_H_
#define _DUNGEON_STAGE_DATA_H_

#include <string>
#include <vector>
#include <unordered_map>


enum class ResourceID {
    Stage1,
    StageBone,
    Player,
    Enemy1,
    Boss,
    BGM,

};
/* 
 *  ダンジョンステージデータ
 */
class DungeonStageData {
private:
    std::unordered_map<ResourceID, std::string> dungeonDataList;        // ステージデータのマップ

public:
    /*
     *  ステージデータの取得
     *  @param[in]  ResourceID  setResourceID   取得したいキー
     *  @return     std::string 
     */
    inline std::string GetResourcePath(ResourceID setResourceID) const { return dungeonDataList.at(setResourceID); }
    /*
     *  データの登録
     *  @param[in]  const ResourceID  setResourceID 登録するキー
     *  @param[in]  const std::string setFilePath   登録するファイルパス
     */
    inline void Register(const ResourceID  setResourceID, const std::string setFilePath) { dungeonDataList[setResourceID] = setFilePath; }

};
#endif // !_DUNGEON_STAGE_DATA_H_
