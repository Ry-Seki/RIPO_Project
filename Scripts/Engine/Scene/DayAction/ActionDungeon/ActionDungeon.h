/*
 *	@file	ActionDungeon.h
 *	@author	Seki
 */

#ifndef _ACTION_DUNGEON_H_
#define _ACTION_DUNGEON_H_

#include "../DayActionBase.h"
#include "../../../../Data/DungeonStageData.h"
#include "../../../../Data/DungeonFloorData.h"
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
	std::shared_ptr<LoadModel> bossResource;
	std::vector<std::shared_ptr<LoadModel>> treasureResource;
	std::shared_ptr<LoadModel> eventTreasureResource;
};

// 前方宣言
class GameObject;

/*
 *	@brief	ダンジョンアクションクラス
 */
class ActionDungeon : public DayActionBase {
private:
	DungeonStageData stageData;												// ステージデータ
	DungeonFloorData floorData;												// フロアデータ
	std::vector<std::vector<std::shared_ptr<GameObject>>> enemyFloorList;	// 階層ごとの敵のリスト
	GameObject* haveTreasure = nullptr;										// プレイヤーが所持しているお宝

	int currentFloor = -1;								// 現在の階層
	bool inputHandle = false;							// 入力フラグ
	bool isChangeFloor = false;							// 階層変更フラグ

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

private:
	/*
	 *	@brief		階層の変更処理
	 */
	void ChangeFloor();
	/*
	 *	@brief		現在の階層の片付け処理
	 */
	void TeardownCurrentFloor();
	/*
	 *	@brief		次の階層の準備
	 */
	void SetupNextFloor();
	/*
	 *	@brief		敵の片付け処理
	 */
	void TeardownEnemy();
	/*
	 *	@brief		ステージオブジェクトの片付け処理
	 */
	void TeardownStageObject();

public:

	void DebugInitialize(Engine& engine, DungeonStageData& setStageData, DungeonFloorData& setFloorData);
	void DebugSetupData(Engine& engine, const DungeonResource& setResource);
	void EndDungeon();
private:
	/*
	 *	@brief		ステージデータからロードリストに追加
	 *	@param[in]	DungeonStageData& stageData			ステージデータ
	 *	@param[in]	DungeonResource&  dungeonResource	セットするリソース
	 */
	void LoadResourcesFromStageData(Engine& engine, DungeonStageData& stageData, DungeonResource& dungeonResource);
public:
	/*
	 *	@brief		ダンジョンステージデータの設定
	 */
	inline void SetDungeonStageData(DungeonStageData setData) { stageData = setData; }
};
#endif // !_ACTION_DUNGEON_H_
