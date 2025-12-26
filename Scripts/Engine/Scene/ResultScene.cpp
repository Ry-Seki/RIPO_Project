/*
 *	@file	ResultScene.cpp
 *	@author	Seki
 */

#include "ResultScene.h"
#include "TitleScene.h"
#include "../Engine.h"

 /*
  *	コンストラクタ
  */
ResultScene::ResultScene() {
}
/*
 *	デストラクタ
 */
ResultScene::~ResultScene() {
}
/*
 *	初期化処理
 */
void ResultScene::Initialize(Engine& engine) {
	engine.SetNextScene(std::make_shared<TitleScene>());
}
/*
 *	ロード済みデータのセット (コールバック)
 */
void ResultScene::SetupData(Engine& engine) {
}
/*
 *	@brief	準備前処理
 */
void ResultScene::Setup() {
}
/*
 *	更新処理
 */
void ResultScene::Update(Engine& engine, float deltaTime) {
}
/*
 *	描画処理
 */
void ResultScene::Render() {
}
