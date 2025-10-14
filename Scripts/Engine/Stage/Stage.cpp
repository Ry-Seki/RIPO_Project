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
	: StageBase() {
	modelHandle = -1;
}

/*
 *  デストラクタ
 */
Stage::~Stage() {
	Execute();
}

/*
 *  ステージ読み込み
 *  @param csvPath 読み込み対象CSVパス
 */
void Stage::Load(const std::string& csvPath) {
	modelPath = csvPath; // 仮処理
	modelHandle = MV1LoadModel(modelPath.c_str());
}

/*
 *	更新
 */
void Stage::Update() {
	// ステージのオブジェクト更新
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
 *	終了処理
 */
void Stage::Execute() {
	Clean(modelHandle);
	if (modelHandle >= 0) {
		MV1DeleteModel(modelHandle);
		modelHandle = -1;
	}
}

/*
 *	当たり判定の更新
 */
void Stage::UpdateCollision() {
	// 衝突判定処理
}