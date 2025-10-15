/*
 *	@file	ActionDungeon.h
 *	@author	Seki
 */

#ifndef _ACTION_DUNGEON_H_
#define _ACTION_DUNGEON_H_

#include "../DayActionBase.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/Model/LoadModel.h"
#include "../../../Load/CSV/LoadCSV.h"

#include <memory>

/*
 *	ダンジョンアクションクラス
 */
class ActionDungeon : public DayActionBase {
private:
	std::shared_ptr<LoadModel> dungeonModel;
	std::shared_ptr<LoadCSV> dungeonData;
	std::shared_ptr<LoadModel> enemyModel;
	std::shared_ptr<LoadModel> bossModel;

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
	void Initialize() override;
	/*
	 *  ロード済みのデータをセット(コールバック)
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

};
#endif // !_ACTION_DUNGEON_H_
