/*
 *  @file   WorldProgressManager.cpp
 *  @author Seki
 */

#include "WorldProgressManager.h"
#include "../../../Data/SaveData.h"
#include "../../../Data/Dungeon/DungeonProgressData.h"

/*
 *	@brief		ワールド進行データの取得
 *  @return		WorldProgressData&
 */
WorldProgressData& WorldProgressManager::GetSaveData() const {
    assert(worldData && "ワールド進行データが設定されていません");
    return *worldData;
}
/*
 *	@brief		ワールド進行データの設定
 *  @param[in]	WorldProgressData& setWorldData
 */
void WorldProgressManager::SetWorldProgressData(WorldProgressData& setWorldData) {
    worldData = &setWorldData;
}
/*
 *	@brief		ダンジョンデータの取得
 *	@param[in]	int dungeonID
 */
DungeonProgressData& WorldProgressManager::GetDungeonProgressData(int dungeonID) {
    assert(worldData && "ワールド進行データが見つかりませんでした");

    auto& map = worldData->dungeonProgress;
    auto it = map.find(dungeonID);

    if (it == map.end()) {
        DungeonProgressData data{};
        data.dungeonID = dungeonID;
        data.isBossDefeated = false;
        it = map.emplace(dungeonID, std::move(data)).first;
    }
    return it->second;
}
/*
 *	@brief		ボス討伐判定
 *  @param[in]	int dungeonID
 *	@return		bool
 */
bool WorldProgressManager::IsBossDefeated(int dungeonID) const {
    assert(worldData && "ワールド進行データが見つかりませんでした");
    if (!worldData) return false;

    auto it = worldData->dungeonProgress.find(dungeonID);
    if (it == worldData->dungeonProgress.end()) {
        return false;
    }
    return it->second.isBossDefeated;
}
/*
 *	@brief		ボス討伐フラグの変更
 *	@param[in]	int dungeonID
 */
void WorldProgressManager::SetIsBossDefeated(int dungeonID) {
    auto& dungeon = GetDungeonProgressData(dungeonID);
    dungeon.isBossDefeated = true;
}
/*
 *	@brief		お宝獲得判定
 *  @param[in]	int dungeonID
 *	@param[in]	int treasureID
 *  @return		bool
 */
bool WorldProgressManager::IsTreasureProcure(int dungeonID, int treasureID) const {
    assert(worldData && "ワールド進行データが見つかりませんでした");
    if (!worldData) return false;
    // ダンジョン進捗データの取得
    auto itr = worldData->dungeonProgress.find(dungeonID);
    if (itr == worldData->dungeonProgress.end()) return false;
    // お宝状況の取得
    auto tItr= itr->second.treasureFlagMap.find(treasureID);
    if (tItr == itr->second.treasureFlagMap.end()) return false;
    return tItr->second;
}
/*
 *	@brief		イベントお宝獲得判定
 *	@param[in]	int dungeonID
 *	@param[in]	int treasureID
 *	@return		bool
 */
bool WorldProgressManager::IsEventTreasureProcure(int dungeonID, int treasureID) const {
    assert(worldData && "ワールド進行データが見つかりませんでした");
    if (!worldData) return false;
    // ダンジョン進捗データの取得
    auto itr = worldData->dungeonProgress.find(dungeonID);
    if (itr == worldData->dungeonProgress.end()) return false;
    // イベントお宝状況の取得
    auto tItr = itr->second.eventTreasureFlagMap.find(treasureID);
    if (tItr == itr->second.eventTreasureFlagMap.end()) return false;
    return tItr->second;
}
/*
 *	@brief		お宝獲得処理
 *  @param[in]	int dungeonID
 *	@param[in]	int treasureID
 */
void WorldProgressManager::ProcureNewTreasure(int dungeonID, int treasureID) {
    auto& dungeon = GetDungeonProgressData(dungeonID);
    if (dungeon.treasureFlagMap[treasureID]) return;

    dungeon.treasureFlagMap[treasureID] = true;
}
/*
 *	@brief		イベントお宝獲得処理
 *	@param[in]	int dungeonID
 *	@param[in]	int treasureID
 */
void WorldProgressManager::ProcureEventTreasure(int dungeonID, int treasureID) {
    auto& dungeon = GetDungeonProgressData(dungeonID);
    if (dungeon.eventTreasureFlagMap[treasureID]) return;

    dungeon.eventTreasureFlagMap[treasureID] = true;
}