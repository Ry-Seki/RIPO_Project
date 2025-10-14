/*
 *  @file Stage.cpp
 *  @author oorui
 */

#include "Stage.h"
#include "../VecMath.h"
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
	// キャッシュ
	modelPath = csvPath;
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
	/// 衝突判定処理
	/// モデルにあるトライアングルリストの数分繰り返す
	//or (int i = 0; i < MV1GetTriangleListNum(modelHandle); i++) {
	//	// トライアングルリストの数分繰り返す
	//	for (int j = 0; j < MV1GetTriangleListPolygonNum(modelHandle, i); j++) {
	//		// 三角形を構成する三頂点の取得
	//		VECTOR vertexs[3] = {};
	//		MV1GetTriangleListPolygonVertexPosition(modelHandle, i, j, vertexs);
	//
	//		// 元データとインスタンス化したデータの位置ずれを考慮
	//		vertexs[0] = position + vertexs[0];
	//		vertexs[1] = position + vertexs[1];
	//		vertexs[2] = position + vertexs[2];
	//
	//		// 接地オブジェクト毎に計算する
	//		for (auto pObj : onGroundObjetArray) {
	//
	//			Vector3 rayOrigin = pObj->GetPosition(), VUp, 120;
	//			Vector3 rayEnd = pObj->GetPosition(), VDown, 150;
	//
	//			// レイを飛ばし、ポリゴンとの当たりを検知する
	//			auto hit = HitCheck_Line_Triangle(
	//				rayOrigin, rayEnd,
	//				vertexs[0], vertexs[1], vertexs[2]
	//			);
	//
	//			// 当たっていたら地面に乗せる
	//			if (hit.HitFlag) {
	//				pObj->SetPosition(
	//					pObj->GetPosition().x,
	//					hit.Position.y,
	//					pObj->GetPosition().z
	//				);
	//			}
	//		}
	//
	//
	//	}
	//
}