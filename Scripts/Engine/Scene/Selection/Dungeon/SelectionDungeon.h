/*
 *	@file	SelectionDungeon.h
 *	@author	Seki
 */

#ifndef _SELECTION_DUNGEON_H_
#define _SELECTION_DUNGEON_H_

#include "../SelectionBase.h"
#include "../../../../Data/DungeonData.h"
#include "../../../../Data/DungeonStageData.h"
#include "../../../../Data/DungeonFloorData.h"
#include "../../../Load/CSV/LoadCSV.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../Load/Dungeon/DungeonDataLoader.h"

#include <vector>
#include <functional>
#include <string>

// 前方宣言
class Engine;

/*
 *	ダンジョン選択クラス
 */
class SelectionDungeon : public SelectionBase {
private:
	std::vector<DungeonData> dungeonDataList;								// ダンジョンデータ配列
	std::shared_ptr<DungeonDataLoader> dungeonDataLoader;					// ダンジョン用読み込みクラス
	int dungeonID = -1;														// 選択されたダンジョンID
	bool inputHandle = false;												// 入力フラグ
	bool isLoad = false;

	std::function<void(Engine& engine, 
					   DungeonStageData setStageData, 
					   DungeonFloorData setFloorData)> ActiveDungeon;		// 行動管理クラスのダンジョン開放関数

	static constexpr const char* _DUNGEON_FLOOR_PATH = "Data/Dungeon/DungeonFloorData.json";

public:
	/*
	 *	デストラクタ
	 */
	~SelectionDungeon() override = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	ロード済みデータのセット（コールバック）
	 */
	void SetupData(Engine& engine) override;
	/*
	 *	更新処理
	 */
	void Update(Engine& engine, float deltaTime) override;
	/*
	 *	描画処理
	 */
	void Render() override;

private:
	/*
	 *	ダンジョンステージデータ読み込み開始
	 *  param[in]	int dungeonID		ダンジョンID
	 */
	void StartStageDataLoad(Engine& engine, int dungeonID);
	/*
	 *	読み込んだステージデータをセットし、アクションマネージャーに渡す(コールバック)
	 *  param[in]	const std::vector<std::shared_ptr<LoadJSON>>& setDataList	ロードしたJSONデータリスト
	 */
	void SetStageData(Engine& engine, const std::vector<std::shared_ptr<LoadJSON>>& setDataList);

public:
	/*
	 *	ActionManagerのダンジョン管理関数の設定
	 *	@param[in]	std::function<void(Engine& engine, DungeonStageData setStageData, DungeonFloorData setFloorData)> 
	 *				setActiveDungeon	行動管理クラスのダンジョン解放関数
	 */
	inline void SetActiveDungeon(std::function<void(Engine& engine, 
								 DungeonStageData setStageData, DungeonFloorData setFloorData)> setActiveDungeon) {
		ActiveDungeon = setActiveDungeon;
	}
};

#endif // !_SELECTION_DUNGEON_H_