/*
 *	@file	ActionDungeon.h
 *	@author	Seki
 */

#ifndef _ACTION_DUNGEON_H_
#define _ACTION_DUNGEON_H_

#include "../DayActionBase.h"
#include "../../../../Data/DungeonData.h"
#include "../../../../Data/DungeonStageData.h"
#include "../../../Load/Model/LoadModel.h"

#include <vector>
#include <memory>
#include <string>

class DungeonDataLoader; // 前方宣言

/*
 *	ダンジョンアクションクラス
 */
class ActionDungeon : public DayActionBase {
private:
	std::shared_ptr<DungeonDataLoader> dungeonDataLoader;
	std::vector<DungeonData> dungeonList;
	DungeonData currentDungeon;
	DungeonStageData stageData;

	std::string debugPath;
	bool inputHandle = false;

public:
	/*
	 *	コンストラクタ
	 */
	ActionDungeon() : DayActionBase("Dungeon") {}
	/*
	 *	デストラクタ
	 */
	~ActionDungeon() override {};

public:
	/*
	 *	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *  ロード済みのデータをセット(コールバック)
	 */
	void Setup(Engine& engine) override;
	/*
	 *	更新処理
	 */
	void Update(Engine& engine, float deltaTime) override;
	/*
	 *	描画処理
	 */
	void Render() override;
	/*
	 *	破棄処理
	 */
	void Teardown() override;

	void DebugInitialize(Engine& engine, DungeonStageData& setStageData);
	void DebugSetup(Engine& engine, std::unordered_map <int, std::shared_ptr<LoadModel>> setModelMap);

public:
	/*
	 *	ダンジョンステージデータの設定
	 */
	inline void SetDungeonStageData(DungeonStageData setData) { stageData = setData; }
	inline void SetDungeonStagePath(std::string setFilePath) { debugPath = setFilePath; }
};
#endif // !_ACTION_DUNGEON_H_
