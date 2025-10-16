/*
 *  @file Stage.cpp
 *  @author oorui
 */

#include "Stage.h"
#include "../VecMath.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Load/Model/LoadModel.h"
#include "../GameConst.h"

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
void Stage::Load(const std::shared_ptr<LoadModel>& model) {

	modelHandle = model->GetHandle();
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
 * @brief ステージの当たり判定を更新
 * @param position     おｚぶてｋとお位置
 * @param PolyPos1     カプセル当たり判定下端
 * @param PolyPos2     カプセル当たり判定上端
 * @param MoveVec      移動ベクトル
 */
void Stage::UpdateCollision(Vector3* position, Vector3 PolyPos1, Vector3 PloyPos2, Vector3 MoveVec) {


	Vector3 prevPos = *position;
	Vector3 nowPos = prevPos + MoveVec;
	float polyOffset = PloyPos2.y - PolyPos1.y;
	bool moveFlag = (Vector3::fabs(MoveVec.x) > 0.01f || Vector3::fabs(MoveVec.z) > 0.01f);

	// コリジョン情報取得
	auto hitDim = SetupCollision(position, MoveVec);

	// 壁床ポリゴン分類
	std::vector<MV1_COLL_RESULT_POLY*> walls, floors;
	ClassifyPolygons(*hitDim, prevPos, walls, floors);

	// 壁衝突処理
	ProcessWallCollision(nowPos, prevPos, polyOffset, MoveVec, walls, moveFlag);

	// 床衝突処理
	ProcessFloorCollision(nowPos, polyOffset, floors);

	// 結果反映
	*position = nowPos;
	MV1CollResultPolyDimTerminate(*hitDim);

}

/*
 * @brief 周囲ポリゴンの当たり判定を実行し、結果を返す
 * @param position  プレイヤー位置
 * @param MoveVec   移動ベクトル
 * @return std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> コリジョン結果構造体
 */
std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> Stage::SetupCollision(Vector3* position, Vector3 MoveVec) {
	// Vector3からVECOTR型へ変換
	VECTOR posV = Vector3::ToVECTOR(*position);

	// 球の半径を求める
	float radius = GameConst::PLAYER_ENUM_DEFAULT_SIZE + MoveVec.Magnitude();

	// ポリゴン配列を格納する構造体を生成
	auto hitDim = std::make_unique<MV1_COLL_RESULT_POLY_DIM>();

	// モデルに対して球との当たり判定を追加
	*hitDim = MV1CollCheck_Sphere(modelHandle, -1, posV, radius);
	return hitDim;
}


/*
 * @brief 壁ポリゴン／床ポリゴンの振り分け
 * @param hitDim   コリジョン結果構造体
 * @param prevPos  移動前の座標
 * @param walls    壁ポリゴン格納先
 * @param floors   床ポリゴン格納先
 */
void Stage::ClassifyPolygons(
	const MV1_COLL_RESULT_POLY_DIM& hitDim,
	const Vector3& prevPos,
	std::vector<MV1_COLL_RESULT_POLY*>& walls,
	std::vector<MV1_COLL_RESULT_POLY*>& floors) {

	// 全ポリゴンをチェック
	for (int i = 0; i < hitDim.HitNum; i++) {
		const auto& poly = hitDim.Dim[i];
		// 壁か床かの判断を行う
		bool isWall = (poly.Normal.y < 0.7f && poly.Normal.y > -0.7f);

		if (isWall) {
			// 壁の高さチェック（プレイヤーより上）
			if (poly.Position[0].y > prevPos.y + 0.3f ||
				poly.Position[1].y > prevPos.y + 0.3f ||
				poly.Position[2].y > prevPos.y + 0.3f) {

				// 壁ポリゴンのポインタを配列に追加
				walls.push_back(const_cast<MV1_COLL_RESULT_POLY*>(&poly));
			}
		}
		else {
			// 床ポリゴンをポインタ配列に追加
			floors.push_back(const_cast<MV1_COLL_RESULT_POLY*>(&poly));
		}
	}

}


/*
 * @brief 壁ポリゴンとの衝突・スライド・押し出し処理
 * @param nowPos      現在座標（処理結果として更新）
 * @param prevPos     移動前座標
 * @param polyOffset  カプセル縦方向のオフセット
 * @param MoveVec     移動ベクトル
 * @param walls       壁ポリゴン配列
 * @param moveFlag    水平移動しているかどうか
 */
void Stage::ProcessWallCollision(
	Vector3& nowPos,
	const Vector3& prevPos,
	float polyOffset,
	const Vector3& MoveVec,
	const std::vector<MV1_COLL_RESULT_POLY*>& walls,
	bool moveFlag) {

	if (walls.empty()) return;

	bool hitFlag = false;

	// 移動していた場合
	if (moveFlag) {
		for (auto* poly : walls) {
			Vector3 capsuleStart = nowPos;
			Vector3 capsuleEnd = nowPos + Vector3(0.0f, polyOffset, 0.0f);
			if (!HitCheck_Capsule_Triangle(
				Vector3::ToVECTOR(capsuleStart),
				Vector3::ToVECTOR(capsuleEnd),
				GameConst::PLAYER_HIT_WIDTH,
				poly->Position[0],
				poly->Position[1],
				poly->Position[2])) continue;

			hitFlag = true;

			// スライドベクトル計算
			Vector3 slideVec = Vector3::Cross(MoveVec, Vector3::FromVECTOR(poly->Normal));
			slideVec = Vector3::Cross(Vector3::FromVECTOR(poly->Normal), slideVec);
			nowPos = prevPos + slideVec;

			// 再衝突確認
			for (auto* polyCheck : walls) {
				Vector3 capsulePos = nowPos + Vector3(0.0f, polyOffset, 0.0f);
				if (HitCheck_Capsule_Triangle(
					Vector3::ToVECTOR(nowPos),
					Vector3::ToVECTOR(capsulePos),
					GameConst::PLAYER_HIT_WIDTH,
					polyCheck->Position[0],
					polyCheck->Position[1],
					polyCheck->Position[2])) {
					return;
				}
			}
			hitFlag = false;
			break;
		}
	}
	else {
		// 止まっている時
		for (auto* poly : walls) {
			Vector3 capsulePos = nowPos + Vector3(0.0f, polyOffset, 0.0f);
			if (HitCheck_Capsule_Triangle(
				Vector3::ToVECTOR(nowPos),
				Vector3::ToVECTOR(capsulePos),
				GameConst::PLAYER_HIT_WIDTH,
				poly->Position[0],
				poly->Position[1],
				poly->Position[2])) {
				hitFlag = true;
				break;
			}
		}
	}

	// 押し出し処理
	if (hitFlag) {
		for (int k = 0; k < GameConst::HIT_TRYNUM; k++) {
			for (auto* poly : walls) {
				Vector3 capsulePos = nowPos + Vector3(0.0f, polyOffset, 0.0f);
				if (!HitCheck_Capsule_Triangle(
					Vector3::ToVECTOR(nowPos),
					Vector3::ToVECTOR(capsulePos),
					GameConst::PLAYER_HIT_WIDTH,
					poly->Position[0],
					poly->Position[1],
					poly->Position[2])) continue;

				Vector3 norm = Vector3::FromVECTOR(poly->Normal).Normalized();
				nowPos += norm * GameConst::HIT_SLIDE_LENGTH;
			}
		}
	}

}


/*
 * @brief 床ポリゴンとの衝突処理
 * @param nowPos      現在座標
 * @param polyOffset  カプセル縦方向のオフセット
 * @param floors      床ポリゴン配列
 */
void Stage::ProcessFloorCollision(
	Vector3& nowPos,
	float polyOffset,
	const std::vector<MV1_COLL_RESULT_POLY*>& floors) {

	if (floors.empty()) return;

	float MaxY = 0.0f;
	bool hitFlag = false;

	for (auto* poly : floors) {
		Vector3 line1 = nowPos + Vector3(0.0f, polyOffset, 0.0f);
		Vector3 line2 = nowPos + Vector3(0.0f, -0.3f, 0.0f);
		HITRESULT_LINE res = HitCheck_Line_Triangle(
			Vector3::ToVECTOR(line1),
			Vector3::ToVECTOR(line2),
			poly->Position[0],
			poly->Position[1],
			poly->Position[2]);

		if (!res.HitFlag) continue;
		if (hitFlag && MaxY > res.Position.y) continue;

		hitFlag = true;
		MaxY = res.Position.y;
	}

	if (hitFlag) nowPos.y = MaxY;

}
