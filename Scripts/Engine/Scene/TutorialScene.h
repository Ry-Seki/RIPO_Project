/*
 *	@file	TutorialScene.h
 *	@author	Seki
 */

#ifndef _TUTORIAL_SCENE_H_
#define _TUTORIAL_SCENE_H_

#include "Scene.h"
#include "../GameEnum.h"
#include "GameState/GameStateMachine.h"

 // 前方宣言
class Engine;

/*
 *	チュートリアルシーン
 */
class TutorialScene : public Scene {
private:
	std::unique_ptr<GameStateMachine> gameState;

	static constexpr const char* _TREASURE_DATA_PATH = "Data/Treasure/TreasureDataList.json";
	static constexpr const char* _ITEM_DATA_PATH = "Data/Item/ItemCatalogData.json";
	static constexpr const char* _EFFECT_DATA_PATH = "Data/Effect/EffectData.json";

public:
	/*
	 *	コンストラクタ
	 */
	TutorialScene();
	/*
	 *	デストラクタ
	 */
	~TutorialScene();

public:
	/*
	 *	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	ロード済みデータのセット (コールバック)
	 */
	void SetupData(Engine& engine);
	/*
	 *	@brief	準備前処理
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
private:
	/*
	 *  @brief  メインシーン終了処理
	 */
	void EndTutorialScene(Engine& engine);

};

#endif // !_TUTORIAL_SCENE_H_