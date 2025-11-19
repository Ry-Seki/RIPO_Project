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
#include "../Component/CapsuleCollider.h"
#include "../Component/GravityComponent.h"
#include "../GameObject.h"
 /*
  *  コンストラクタ
  */
Stage::Stage()
	: StageBase()
	, lightDirection(0.2f, -20.0f, 0.3f)
	, pointLightColor(1.0f, 0.8f, 0.6f)  // 色
	, pointLightRange(10000.0f)            // ポイントライトの範囲
{
	modelHandle = -1;
}

/*
 *  デストラクタ
 */
Stage::~Stage() {
	Execute();
}


/*
 *	ステージデータの読み込み
 *  @pram const int modelHandle
 */
void Stage::SetModelHandle(const int modelHandleBase) {
	modelHandle = modelHandleBase;

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
		// ライトをセット
		LightSettings();
		// モデルの描画
		MV1DrawModel(modelHandle);
#if _DEBUG
		// ステージの当たり判定の描画
		//StageColliderRenderer();
#endif // _DEBUG


	}
}

/*
 *	終了処理
 */
void Stage::Execute() {
	if (modelHandle >= 0) {
		// モデルを非表示
		Clean(modelHandle);
		// モデルの片付け
		MV1DeleteModel(modelHandle);
		// モデルをnull
		modelHandle = -1;
	}
}


/*
 * @brief ステージの当たり判定を更新
 * @param position     オブジェクト位置	参照渡しで座標変更
 * @param MoveVec      移動ベクトル
 */
void Stage::UpdateCollision(GameObject* other, Vector3 MoveVec) {

	// 移動前の座標
	Vector3 prevPos = other->position;
	Vector3 currentPos = prevPos + MoveVec;

	// プレイヤーの座標
	Vector3 PolyPos = other->position;
	// PolyPosTopより指定座標分高い位置
	Vector3 PolyPosTop = other->position;
	PolyPosTop.y += GameConst::PLAYER_HIT_HEIGHT;

	// 縦方向のオフセット
	float polyOffset = PolyPosTop.y - PolyPos.y;
	// 水平移動しているかどうか
	bool moveFlag = (fabs(MoveVec.x) > 0.01f || fabs(MoveVec.z) > 0.01f);

	// コリジョン情報取得
	auto hitDim = SetupCollision(other->position, MoveVec);

	// 壁床ポリゴン分類
	std::vector<MV1_COLL_RESULT_POLY*> walls, floors;
	ClassifyPolygons(*hitDim, prevPos, walls, floors);

	// 壁衝突処理
	ProcessWallCollision(currentPos, prevPos, polyOffset, MoveVec, walls, moveFlag);

	// 床衝突処理
	ProcessFloorCollision(currentPos, polyOffset, floors, other, MoveVec.y);

	// 結果反映
	other->position = currentPos;
	MV1CollResultPolyDimTerminate(*hitDim);

}

/*
 * @brief 周囲ポリゴンの当たり判定を実行し、結果を返す
 * @param position  プレイヤー位置
 * @param MoveVec   移動ベクトル
 * @return std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> コリジョン結果構造体
 */
std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> Stage::SetupCollision(Vector3 position, Vector3 MoveVec) {
	// Vector3からVECOTR型へ変換
	VECTOR posV = ToVECTOR(position);

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
		bool isWall = (poly.Normal.y < 0.9f && poly.Normal.y > -0.9f);

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

	// 壁が存在しなければ抜ける
	if (walls.empty()) return;

	// カプセル設定
	const float capsuleRadius = GameConst::PLAYER_HIT_HEIGHT;

	// カプセルの下端
	Vector3 capLower = nowPos;
	// カプセルの上端
	Vector3 capTop = nowPos + Vector3(0, polyOffset, 0);

	for (auto* poly : walls) {
		// 三角形の 3 頂点
		Vector3 p0 = FromVECTOR(poly->Position[0]);
		Vector3 p1 = FromVECTOR(poly->Position[1]);
		Vector3 p2 = FromVECTOR(poly->Position[2]);

		// カプセル中心点
		Vector3 capCenter = (capLower + capTop) * 0.5f;

		// 最近接点を求める
		Vector3 nearest = Nearest(capCenter, p0, p1, p2);

		// どれだけ貫通しているかを求める
		Vector3 diff = capCenter - nearest;
		float dist = Magnitude(diff);

		// カプセルが触れていないなら処理しない
		float penetration = capsuleRadius - dist;
		if (penetration <= 0.0f) continue;


		Vector3 pushDir = Normalized(diff);

		// 貫通した分戻す
		nowPos += pushDir * penetration;

		// 新しいカプセル位置
		capLower = nowPos;
		capTop = nowPos + Vector3(0, polyOffset, 0);
	}

	if (moveFlag) {
		// 元の移動ベクトル
		Vector3 moveVec = MoveVec;

		// 壁の向きを一つにまとめる
		Vector3 avgNormal = Vector3::zero;
		int count = 0;

		// ポリゴンの法線を足していく
		for (auto* poly : walls) {
			// 法線をVector3型に直し、乗算
			avgNormal += FromVECTOR(poly->Normal);
			count++;
		}
		// 正規化した長さに変換
		avgNormal = Normalized((avgNormal / (float)count));

		// 移動ベクトルと壁の向きの内積
		float dot = Vector3::Dot(moveVec, avgNormal);

		// 壁に沿った移動に変更
		Vector3 slideVec = moveVec - avgNormal * dot;

		// 対象者の位置を更新
		//nowPos = prevPos + slideVec;
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
	const std::vector<MV1_COLL_RESULT_POLY*>& floors,
	GameObject* other,
	float moveVec
) {

	if (floors.empty()) return;

	float MaxY = 0.5f;
	bool isGrounded = false;
	auto hitGrounding = other->GetComponent<GravityComponent>();


	for (auto* poly : floors) {
		Vector3 line1 = nowPos + Vector3(0.0f, polyOffset, 0.0f);
		Vector3 line2 = nowPos + Vector3(0.0f, -0.3f, 0.0f);
		// 三角形と線分の当たり判定
		HITRESULT_LINE res = HitCheck_Line_Triangle(
			ToVECTOR(line1),
			ToVECTOR(line2),
			poly->Position[0],
			poly->Position[1],
			poly->Position[2]);

		if (!res.HitFlag) continue;
		if (!isGrounded || MaxY < res.Position.y) {
			isGrounded = true;
			MaxY = res.Position.y;
		}
	}

	// ジャンプ中は判定しない
	if (moveVec > 0.1f) {
		hitGrounding->SetGroundingFrag(false);
		return;
	}
	// 判定する
	hitGrounding->SetGroundingFrag(isGrounded);
	if (hitGrounding->GetGroundingFrag()) nowPos.y = MaxY;

}

/*
 *	ステージのライトの設定
 */
void Stage::LightSettings() {

	// 古いライトを全て削除
	DeleteLightHandleAll();

	// マップ全体のライト設定
	// アンビエントカラーの設定
	SetLightAmbColor(_MAP_AMB_COLOR);
	// ライトの方向を設定する
	SetLightDirection(ToVECTOR(lightDirection));
	SetLightDifColor(_MAP_DIF_COLOR);
	SetLightSpcColor(_MAP_SPC_COLOR);

	// ポイントライト設定
	pointLightPos = StageManager::GetInstance().GetPointLightPos();
	// ポイントライトがおける場所がある分、ポイントライトを配置する
	for (const auto& pos : pointLightPos) {
		// ポイントライトのハンドルを作成
		int pLight = CreatePointLightHandle(ToVECTOR(pos), pointLightRange, _POINT_ATTAN.Atten0, _POINT_ATTAN.Atten1, _POINT_ATTAN.Atten2);
		// ポイントライトの色を指定する
		SetLightDifColorHandle(pLight, GetColorF(pointLightColor.x, pointLightColor.y, pointLightColor.z, Vector3::one.x));
		SetLightSpcColorHandle(pLight, _POINT_SPC_COLOR);
		SetLightEnableHandle(pLight, TRUE);
	}

}


/*
 *	ステージの当たり判定の描画
 */
void Stage::StageColliderRenderer() {
	// モデルハンドルが無効な場合は処理を中止
	if (modelHandle < 0) return;

	// 当たり判定の中心位置
	VECTOR checkCenter = ToVECTOR(Vector3::zero);

	// 判定に使用する球の半径
	float  radius = 20000.0f;

	// モデル全体の当たり判定ポリゴン情報を格納する構造体を動的確保
	auto hitDim = std::make_unique<MV1_COLL_RESULT_POLY_DIM>();

	// 球とモデルの衝突判定を行い、結果を取得
	*hitDim = MV1CollCheck_Sphere(modelHandle, -1, checkCenter, radius);

	// 壁と床を区別するための色情報を設定
	unsigned int wallColor = GetColor(255, 100, 100);
	unsigned int floorColor = GetColor(100, 255, 100);
	unsigned int lineColor = GetColor(255, 255, 255);

	// 当たった全てのポリゴンに対して描画処理を実行
	for (int i = 0; i < hitDim->HitNum; i++) {
		const auto& poly = hitDim->Dim[i];

		// 壁／床を判定
		bool isWall = (poly.Normal.y < 0.9f && poly.Normal.y > -0.9f);
		unsigned int drawColor = isWall ? wallColor : floorColor;

		// ポリゴンを半透明で描画
		DrawTriangle3D(poly.Position[0], poly.Position[1], poly.Position[2], drawColor, TRUE);

		// ポリゴンを白線で描画
		DrawLine3D(poly.Position[0], poly.Position[1], lineColor);
		DrawLine3D(poly.Position[1], poly.Position[2], lineColor);
		DrawLine3D(poly.Position[2], poly.Position[0], lineColor);
	}

	// 使用した衝突判定データを解放
	MV1CollResultPolyDimTerminate(*hitDim);
}