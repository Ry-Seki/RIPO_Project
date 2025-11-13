/*
 *	@file	ActionDungeon.h
 *	@author	Seki
 */

#ifndef _ACTION_DUNGEON_H_
#define _ACTION_DUNGEON_H_

#include "../DayActionBase.h"
#include "../../../../Data/DungeonStageData.h"
#include "../../../Load/Model/LoadModel.h"
#include "../../../Load/JSON/LoadJSON.h"
#include <vector>
#include <memory>
#include <string>

/*
 *	各ロードデータ構造体
 */
struct DungeonResource {
	std::vector<std::shared_ptr<LoadModel>> stageResource;
	std::vector<std::shared_ptr<LoadJSON>> stageBoneResource;
	std::shared_ptr<LoadModel> playerResource;
	std::vector<std::shared_ptr<LoadModel>> enemyResource;
	std::shared_ptr<LoadModel> bulletResource;
	std::shared_ptr<LoadModel> bossResource;
	std::vector<std::shared_ptr<LoadModel>> treasureResource;
	std::shared_ptr<LoadModel> eventTreasureResource;
};

/*
 *	ダンジョンアクションクラス
 */
class ActionDungeon : public DayActionBase {
private:
	DungeonStageData stageData;

	bool inputHandle = false;

private:
	static bool isFirst;
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
	void SetupData(Engine& engine) override;
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
	void DebugSetup(Engine& engine, const DungeonResource& setResource);
	void EndDungeon();
private:
	/*
	 *	ステージデータからロードリストに追加
	 *	@param[in]	DungeonStageData& stageData			ステージデータ
	 *	@param[in]	DungeonResource&  dungeonResource	セットするリソース
	 */
	void LoadResourcesFromStageData(Engine& engine, DungeonStageData& stageData, DungeonResource& dungeonResource);
public:
	/*
	 *	ダンジョンステージデータの設定
	 */
	inline void SetDungeonStageData(DungeonStageData setData) { stageData = setData; }
};
#endif // !_ACTION_DUNGEON_H_
