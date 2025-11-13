/*
 *	@file	StandbyScene.h
 *	@author	Seki
 */

#ifndef _STANDBY_SCENE_H_
#define _STANDBY_SCENE_H_

#include "Scene.h"

// 前方宣言
class Engine;

 /*
  *	スタンバイシーン
  */
class StandbyScene : public Scene {

public:
	/*
	 *	コンストラクタ
	 */
	StandbyScene();
	/* 
	 *	デストラクタ
	 */
	~StandbyScene();

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
	 *	更新処理
	 */
	void Update(Engine& engine, float deltaTime) override;
	/*
	 *	描画処理
	 */
	void Render() override;

};

#endif // !_STANDBY_SCENE_H_