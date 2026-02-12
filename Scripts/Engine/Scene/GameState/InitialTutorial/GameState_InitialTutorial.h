/*
 *	@file	GameState_InitialTutorial.h
 *	@author	Seki
 */

#ifndef _GAME_STATE_INITIAL_TUTORIAL_H_
#define _GAME_STATE_INITIAL_TUTORIAL_H_

#include "../GameStateBase.h"
#include "../../../../Data/Dungeon/DungeonData.h"

#include <vector>
#include <memory>

 // 前方宣言
class DungeonDataLoader;
class LoadJSON;

// 前方宣言
class Engine;
/*
 *	@brief	NewGame時のチュートリアル状態
 */
class GameState_InitialTutorial : public GameStateBase {
private:
	std::vector<DungeonData> dungeonDataList;				// ダンジョンデータ配列
	std::shared_ptr<DungeonDataLoader> dungeonDataLoader;	// ダンジョン用読み込みクラス

	static constexpr const char* _DUNGEON_LIST_PATH = "Data/Dungeon/DungeonList.csv";
	static constexpr const char* _DUNGEON_FLOOR_PATH = "Data/Dungeon/DungeonFloorData.json";

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	準備前処理
	 */
	void Setup() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime) override;
	/*
	 *	@brief	描画処理(デバッグ用)
	 */
	void Render() override;
	/*
	 *	@brief	片付け処理
	 */
	void Teardown() override;

private:
	/*
	 *	@brief		ダンジョンデータのロード開始
	 */
	void StartDungeonDataLoad();
	/*
	 *	@brief		読み込んだダンジョンデータの設定
	 *  @param[in]	const std::vector<std::shared_ptr<LoadJSON>>& setDataList
	 */
	void SetDungeonData(const std::vector<std::shared_ptr<LoadJSON>>& setDataList);

};

#endif // !_GAME_STATE_INITIAL_TUTORIAL_H_