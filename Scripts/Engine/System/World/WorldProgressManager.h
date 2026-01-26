/*
 *	@file	WorldProgressManager.h
 *	@author	Seki
 */

#ifndef _WORLD_PROGRESS_MANAGER_H_
#define _WORLD_PROGRESS_MANAGER_H_

#include "../../Singleton.h"

#include <cassert>

// 前方宣言
struct WorldProgressData;
struct DungeonProgressData;

/*
 *	@brief	ダンジョン進捗度を管理するクラス
 */
class WorldProgressManager : public Singleton<WorldProgressManager> {
	friend class Singleton<WorldProgressManager>;

private:
	WorldProgressData* worldData = nullptr;

public:
	/*
	 *	@brief	コンストラクタ
	 */
	WorldProgressManager() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~WorldProgressManager() override = default;

public:
	/*
	 *	@brief		ワールド進行データの設定
	 *  @param[in]	WorldProgressData& setWorldData
	 */
	void SetWorldProgressData(WorldProgressData& setWorldData);
	/*
	 *	@brief		ダンジョンデータの取得
	 *	@param[in]	int dungeonID
	 */
	DungeonProgressData& GetDungeonProgressData(int dungeonID);
	/*
	 *	@brief		ボス討伐判定
	 *  @param[in]	int dungeonID
	 *	@return		bool
	 */
	bool IsBossDefeated(int dungeonID) const;
	/*
	 *	@brief		ボス討伐フラグの変更
	 *	@param[in]	int dungeonID
	 */
	void SetIsBossDefeated(int dungeonID);
	/*
	 *	@brief		お宝獲得判定
	 *  @param[in]	int dungeonID
	 *	@param[in]	int treasureID
	 *  @return		bool
	 */
	bool IsTreasureGet(int dungeonID, int treasureID) const;
	/*
	 *	@brief		お宝獲得処理
	 *  @param[in]	int dungeonID
	 *	@param[in]	int treasureID
	 */
	void GetNewTreasure(int dungeonID, int treasureID);
	/*
	 *	@brief		ワールド進行データの取得
	 *  @return		WorldProgressData&
	 */
	WorldProgressData& GetSaveData() const;

};

#endif // !_WORLD_PROGRESS_MANAGER_H_