/*
 *	@file	SelectionDungeon.h
 *	@author	Seki
 */

#ifndef _SELECTION_DUNGEON_H_
#define _SELECTION_DUNGEON_H_

#include "../SelectionBase.h"
#include "../../../../Data/DungeonData.h"
#include "../../../../Data/DungeonStageData.h"
#include "../../../Load/CSV/LoadCSV.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../Load/Dungeon/DungeonDataLoader.h"

#include <vector>
#include <functional>

/*
 *	ダンジョン選択クラス
 */
class SelectionDungeon : public SelectionBase {
private:
	std::vector<DungeonData> dungeonDataList;
	std::shared_ptr<DungeonDataLoader> dungeonDataLoader;
	bool inputHandle = false;

public:
	/*
	 *	デストラクタ
	 */
	~SelectionDungeon() override = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize() override;
	/*
	 *	ロード済みデータのセット（コールバック）
	 */
	void Setup() override;
	/*
	 *	更新処理
	 */
	void Update(float deltaTime) override;
	/*
	 *	描画処理
	 */
	void Render() override;
private:
	/*
	 *	ダンジョンステージデータ読み込み開始
	 *  param[in]	int dungeonID		ダンジョンID
	 */
	void StartStageDataLoad(int dungeonID);
	/*
	 *	読み込んだステージデータをセットし、アクションマネージャーに渡す(コールバック)
	 *  param[in]	std::shared_ptr<LoadJSON> setData	ロードしたJSONデータ
	 */
	void SetStageData(std::shared_ptr<LoadJSON> setData);

private:
	void DebugStageLoad(int dungeonID);
	void DebugSetStageData(std::shared_ptr<LoadJSON> setData);
public:
	/*
	 *	有効フラグの取得
	 */
	inline bool IsActive() const { return isActive; }

};
#endif // !_SELECTION_DUNGEON_H_
