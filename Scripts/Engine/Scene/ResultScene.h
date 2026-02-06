/*
 *	@file	ResultScene.h
 *	@author	Seki
 */

#ifndef _RESULT_SCENE_H_
#define _RESULT_SCENE_H_

#include "Scene.h"

 // 前方宣言
class Engine;

/*
 *	@brief	リザルトシーン
 */
class ResultScene : public Scene {
private:
	bool isStart = false;
	bool inputHandle = false;

	static constexpr const char* _RESULT_SCORE_PATH = "Data/Treasure/TreasureDataList.json";

public:
	/*
	 *	@brief	コンストラクタ
	 */
	ResultScene(){}
	/*
	 *	@brief	デストラクタ
	 */
	~ResultScene() override {}

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	ロード済みデータのセット (コールバック)
	 */
	void SetupData(Engine& engine);
	/*
	 *	@brief	準備前処理
	 */
	void Setup(Engine& engine) override;
	/*
	 *	@brief	更新処理
	 */
	void Update(Engine& engine, float deltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
};

#endif // !_RESULT_SCENE_H_