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

		StageColliderRenderer(player, playerMove);
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

	// 移動後の位置
	Vector3 nowPos = other->position;
	// 移動前の座標
	Vector3 prevPos = nowPos - MoveVec;

	// プレイヤーの座標
	Vector3 PolyPos = other->position;
	// PolyPosTopより指定座標分高い位置
	Vector3 PolyPosTop = other->position;
	PolyPosTop.y += GameConst::PLAYER_HIT_HEIGHT;

	// 縦方向のオフセット
	float polyOffset = PolyPosTop.y - PolyPos.y;
	// キャラクターが水平移動しているかどうか
	bool moveFlag = (fabs(MoveVec.x) > 0.01f || fabs(MoveVec.z) > 0.01f);

	// ステージのコリジョン情報取得
	auto hitDim = SetupCollision(other, MoveVec);

	// 壁床ポリゴン分類
	std::vector<MV1_COLL_RESULT_POLY*> walls, floors;
	// 壁か床か振り分ける
	ClassifyPolygons(*hitDim, walls, floors);

	// 壁衝突処理
	ProcessWallCollision(nowPos, prevPos, polyOffset, MoveVec, walls, moveFlag, other);

	// 床衝突処理
	ProcessFloorCollision(nowPos, polyOffset, floors, other, MoveVec.y);

	// 結果反映
	other->position = nowPos;
	MV1CollResultPolyDimTerminate(*hitDim);

}

/*
 * @brief 周囲ポリゴンの当たり判定を実行し、結果を返す
 * @param position  プレイヤー位置
 * @param MoveVec   移動ベクトル
 * @return std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> コリジョン結果構造体
 */
std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> Stage::SetupCollision(GameObject* other, Vector3 MoveVec) {
	// 対象がいない場合は抜ける
	if (!other)return std::make_unique<MV1_COLL_RESULT_POLY_DIM>();

	Vector3 effectiveMove = MoveVec;
	if (Magnitude(effectiveMove) < 0.0001f) {
		// 移動していない場合は小さめの球を用意して静止判定用
		effectiveMove = Vector3::zero;
	}
	// カプセルの取得
	auto capsule = other->GetComponent<CapsuleCollider>();
	if (!capsule)return std::make_unique<MV1_COLL_RESULT_POLY_DIM>();

	// 下端
	Vector3 capLower = other->position + capsule->capsule.startPoint;
	// 上端
	Vector3 capTop = other->position + capsule->capsule.endPoint;
	// 中心点
	Vector3 capCenter = (capLower + capTop) * 0.5f;
	// 半径
	float radius = capsule->capsule.radius;

	// 移動方向ベクトルを正規化
	Vector3 forward = Normalized(MoveVec);

	float forwardLength = 2000.0f;		 // 前方判定距離
	float backwardLength = 500.0f;		 // 後退判定距離
	float boxHalfSize = radius + 500.0f;  // 左右/上下余裕


	// 前方限定の球判定用中心
	Vector3 sphereCenter = capCenter + forward * MoveVec.Magnitude() * 0.5f;

	// 球の半径
	float sphereRadius = radius + MoveVec.Magnitude() * 0.5f;

	auto hitDim = std::make_unique<MV1_COLL_RESULT_POLY_DIM>();
	*hitDim = MV1CollCheck_Sphere(modelHandle, -1, ToVECTOR(sphereCenter), sphereRadius);

	return hitDim;
}


/*
 * @brief 壁ポリゴン／床ポリゴンの振り分け
 * @param hitDim   コリジョン結果構造体
 * @param walls    壁ポリゴン格納先
 * @param floors   床ポリゴン格納先
 */
void Stage::ClassifyPolygons(
	const MV1_COLL_RESULT_POLY_DIM& hitDim,
	std::vector<MV1_COLL_RESULT_POLY*>& walls,
	std::vector<MV1_COLL_RESULT_POLY*>& floors) {

	// 全ポリゴンをチェック
	for (int i = 0; i < hitDim.HitNum; i++) {
		const auto& poly = hitDim.Dim[i];
		// 壁か床かの判断を行う
		bool isWall = (poly.Normal.y < 0.9f && poly.Normal.y > -0.9f);

		// 壁かどうか
		if (isWall) {
			// 壁ポリゴン配列に追加する
			walls.push_back(const_cast<MV1_COLL_RESULT_POLY*>(&poly));
		}
		else {
			// 床ポリゴン配列に追加する
			floors.push_back(const_cast<MV1_COLL_RESULT_POLY*>(&poly));
		}
	}

}


/*
 * @brief 壁ポリゴンとの衝突・スライド・押し出し処理
 * @param nowPos      現在座標
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
	bool moveFlag,
	GameObject* other
) {
	// 壁が存在しなければ抜ける
	if (walls.empty()) return;

	// カプセルコンポーネントの取得
	auto capsule = other->GetComponent<CapsuleCollider>();
	// カプセルを取得できなければ抜ける
	if (!capsule)return;

	// カプセルの半径を取得
	float capsuleRadius = capsule->capsule.radius;

	// ワールド座標に変換したカプセルの下端
	Vector3 capLower = nowPos + capsule->capsule.startPoint;
	// ワールド座標に変換したカプセルの上端
	Vector3 capTop = nowPos + capsule->capsule.endPoint;

	// 壁ポリゴンの配列分回す
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
		float penetrate = capsuleRadius - dist;
		if (penetrate <= 0.0f) continue;


		Vector3 pushDir = Normalized(diff);

		// 貫通した分戻す
		nowPos += pushDir * penetrate;

		// 新しいカプセル位置
		capLower = nowPos + capsule->capsule.startPoint;
		capTop = nowPos + capsule->capsule.endPoint;
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
	// 床がなければ落下
	if (floors.empty())return;

	// 接地点の最大値
	float MaxY = 0.1f;
	// 接地しているかどうか
	bool isGround = false;

	// 重力コンポーネントの取得
	auto hitGrounding = other->GetComponent<GravityComponent>();
	if (!hitGrounding) return;

	// カプセルコンポーネント取得
	auto capsule = other->GetComponent<CapsuleCollider>();
	if (!capsule) return;

	// ワールド座標での下端
	Vector3 capStart = nowPos + capsule->capsule.startPoint;
	// ワールド座標での上端
	Vector3 capEnd = nowPos + capsule->capsule.endPoint;
	// カプセル中心点
	Vector3 capCenter = (capStart + capEnd) * 0.5f;
	// カプセルの半径
	float capsuleRadius = capsule->capsule.radius;

	// 貫通しているかどうかを見る
	for (auto* poly : floors) {
		// 三角形の 3 頂点
		Vector3 p0 = FromVECTOR(poly->Position[0]);
		Vector3 p1 = FromVECTOR(poly->Position[1]);
		Vector3 p2 = FromVECTOR(poly->Position[2]);

		// 中心点と三角形の最近接点を求める
		Vector3 nearest = Nearest(capCenter, p0, p1, p2);

		// 最近接点との差分と距離
		Vector3 diff = capCenter - nearest;
		float dist = Magnitude(diff);

		// カプセルの半径以下なら接地
		const float EPS = 0.5f;
		if (dist <= capsuleRadius + EPS) {
			// 接地判定
			if (!isGround || MaxY < nearest.y) {
				isGround = true;
				MaxY = nearest.y;
			}
		}
	}

	// 地面から離れていた場合は接地判定を行わない
	if (hitGrounding->GetFallSpeed() < 0) {
		hitGrounding->SetGroundingFrag(false);
		return;
	}

	// 接地していたら
	if (isGround) {
		// 対象がいなければいけない位置
		float desiredY = MaxY - capsule->capsule.startPoint.y;

		// 壁から押し出されたとき対策
		// 床よりもしたに押し出された場合、押し戻す
		if (nowPos.y < desiredY) {
			nowPos.y = desiredY;
		}
		// 接地している
		hitGrounding->SetGroundingFrag(true);
	}
	else {
		// 接地していない
		hitGrounding->SetGroundingFrag(false);
	}
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
void Stage::StageColliderRenderer(GameObject* other, Vector3 MoveVec) {
	// モデルハンドルが無効な場合は処理を中止
	if (modelHandle < 0) return;

	auto hitDim = SetupCollision(other, MoveVec);

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
		DrawTriangle3D(poly.Position[0], poly.Position[1], poly.Position[2], drawColor, FALSE);

		// ポリゴンを白線で描画
		DrawLine3D(poly.Position[0], poly.Position[1], lineColor);
		DrawLine3D(poly.Position[1], poly.Position[2], lineColor);
		DrawLine3D(poly.Position[2], poly.Position[0], lineColor);
	}

	// 使用した衝突判定データを解放
	MV1CollResultPolyDimTerminate(*hitDim);
}