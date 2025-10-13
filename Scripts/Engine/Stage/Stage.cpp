/*
 *  @file Stage.cpp
 *  @author oorui
 */

#include "Stage.h"
#include <fstream>
#include <sstream>
#include <iostream>

 /*
  *  コンストラクタ
  */
Stage::Stage()
	: modelHandle(-1) {
}

/*
 *  デストラクタ
 */
Stage::~Stage() {
	Execute();
}

/*
 *  ステージ読み込み
 *  @pram std::string& csvPath
 */
void Stage::Load(const std::string& csvPath) {
}

/*
 *	更新
 */
void Stage::Update() {
	// ステージのオブジェクト、敵の配置

}

/*
 *  描画
 */
void Stage::Render() {
	if (modelHandle >= 0) {
		MV1DrawModel(modelHandle);
	}
}

/*
 *	終了
 */
void Stage::Execute() {
}

/*
 *	当たり判定の更新
 */
void Stage::UpdateCollision() {

}
