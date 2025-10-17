/*
 *	@file	ActionDungeon.h
 *	@author	Seki
 */

#ifndef _ACTION_DUNGEON_H_
#define _ACTION_DUNGEON_H_

#include "../DayActionBase.h"
#include "../../../../Data/DungeonData.h"
#include <vector>
#include <memory>

class DungeonDataLoader; // 前方宣言

/*
 *	ダンジョンアクションクラス
 */
class ActionDungeon : public DayActionBase {
private:
	std::shared_ptr<DungeonDataLoader> dungeonDataLoader;
	std::vector<DungeonData> dungeonList;
	DungeonData currentDungeon;

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
