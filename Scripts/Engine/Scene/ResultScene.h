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
 *	リザルトシーン
 */
class ResultScene : public Scene {

public:
	/*
	 *	コンストラクタ
	 */
	ResultScene();
	/*
	 *	デストラクタ
	 */
	~ResultScene();

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
	void Setup() override;
	/*
	 *	更新処理
	 */
	void Update(Engine& engine, float deltaTime) override;
	/*
	 *	描画処理
	 */
	void Render() override;
};

#endif // !_RESULT_SCENE_H_