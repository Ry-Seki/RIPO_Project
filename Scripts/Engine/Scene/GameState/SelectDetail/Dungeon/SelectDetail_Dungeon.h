/*
 *	@file	SelectDetail_Dungeon.h
 *	@author	Seki
 */

#ifndef _SELECT_DETAIL_DUNGEON_H_
#define _SELECT_DETAIL_DUNGEON_H_

#include "../SelectDetailBase.h"
#include "../../../../../Data/Dungeon/DungeonData.h"
#include <vector>
#include <memory>

// 前方宣言
class DungeonDataLoader;
class LoadJSON;

/*
 *	@brief	ダンジョンの内容を選択するクラス
 */
class SelectDetail_Dungeon : public SelectDetailBase {
private:
	std::vector<DungeonData> dungeonDataList;				// ダンジョンデータ配列
	std::shared_ptr<DungeonDataLoader> dungeonDataLoader;	// ダンジョン用読み込みクラス
	int dungeonID = -1;

	static constexpr const char* _DUNGEON_FLOOR_PATH = "Data/Dungeon/DungeonFloorData.json";

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize() override;
	/*
	 *	@breif	ロード済みデータのセット
	 */
	void SetupData();
	/*
	 *	@brief	準備前処理
	 */
	void Setup() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	片付け処理
	 */
	void Teardown() override;

public:
	/*
	 *	@brief		ダンジョンデータのロード開始
	 */
	void StartDungeonDataLoad(int dungeonID);
	/*
	 *	@brief		読み込んだダンジョンデータの設定
	 *  @param[in]	const std::vector<std::shared_ptr<LoadJSON>>& setDataList
	 */
	void SetDungeonData(const std::vector<std::shared_ptr<LoadJSON>>& setDataList);
};

#endif // !_SELECT_DETAIL_DUNGEON_H_