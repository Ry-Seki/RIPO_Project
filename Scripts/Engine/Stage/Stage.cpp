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
 *	当たり判定の更新
 */
void Stage::UpdateCollision(Vector3* position, Vector3 PolyPos1, Vector3 PloyPos2, Vector3 MoveVec) {

	int i, j, k;									// 汎用カウンタ変数
	bool moveFlag;									// 水平方向に移動したかどうかのフラグ
	bool hitFlag = false;							// ポリゴンに当たったかどうか記憶しておく変数
	auto hitDim = std::make_unique<MV1_COLL_RESULT_POLY_DIM>();
	int kabeNum;									// 壁ポリゴンと判断されたポリゴンの数
	int yukaNum;									// 床ポリゴンと判断されたポリゴンの数
	std::vector<MV1_COLL_RESULT_POLY*> kabe;		// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	std::vector<MV1_COLL_RESULT_POLY*> yuka;		// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	kabe.reserve(PLAYER_MAX_HITCOLL);
	yuka.reserve(PLAYER_MAX_HITCOLL);
	MV1_COLL_RESULT_POLY* poly;						// ポリゴン構造体にアクセスするために使用するポインタ
	HITRESULT_LINE lineRes;							// 線分とポリゴンとの当たり判定の結果を代入する構造体
	Vector3 prevPos;
	Vector3 nowPos;
	char prevJumpStatus = 0;
	float polyOffset;

	// 移動前の座標を保存
	prevPos = *position;

	// Poly1とPoly2のY座標の差を保存
	polyOffset = PloyPos2.y - PolyPos1.y;

	// 移動後の座標を算出
	nowPos = prevPos + MoveVec;

	// x軸かz軸に0.01f以上移動した場合はフラグをtrueにする
	moveFlag = (Vector3::fabs(MoveVec.x) > 0.01f || Vector3::fabs(MoveVec.z) > 0.01f) ? true : false;

	// プレイヤーの周囲にあるステージポリゴンを取得する
	VECTOR posV = Vector3::ToVECTOR(*position); // Vector3->VECTORに変換
	float radius = PLAYER_ENUM_DEFAULT_SIZE + MoveVec.Magnitude();
	// コリジョン検出
	*hitDim = MV1CollCheck_Sphere(modelHandle, -1, posV, radius);

	// 検出されたポリゴンが壁ポリゴンか床ポリゴンか判断する
	{
		// 壁ポリゴンと床ポリゴンの数を初期化する
		kabeNum = 0;
		yukaNum = 0;

		// 検出されたポリゴンの数だけ繰り返す
		for (i = 0; i < hitDim->HitNum; i++) {
			// XZ平面に垂直かどうかはポリゴンの法線のY成分の値で判断する
			if (hitDim->Dim[i].Normal.y < 0.7f && hitDim->Dim[i].Normal.y > -0.7f) {
				// 壁ポリゴンと判断された場合でも、プレイヤーのＹ座標＋0.3ｆより高いポリゴンのみ当たり判定を行う
				if (hitDim->Dim[i].Position[0].y > prevPos.y + 0.3f ||
					hitDim->Dim[i].Position[1].y > prevPos.y + 0.3f ||
					hitDim->Dim[i].Position[2].y > prevPos.y + 0.3f) {

					// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
					if (kabeNum < PLAYER_MAX_HITCOLL) {
						// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
						kabe[kabeNum] = &hitDim->Dim[i];

						// 壁ポリゴンの数を加算する
						kabeNum++;
					}
				}
			}
			else {
				// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
				if (yukaNum < PLAYER_MAX_HITCOLL) {
					// ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
					yuka[yukaNum] = &hitDim->Dim[i];

					// 床ポリゴンの数を加算する
					yukaNum++;
				}
			}
		}


	}

	if (kabeNum != 0) {

		// 壁に当たったかどうかのフラグは初期状態では当たっていないにしておく
		hitFlag = false;

		// 移動したかどうかで処理を分岐
		if (moveFlag == true) {
			// 壁ポリゴンの数だけ繰り返し
			for (i = 0; i < kabeNum; i++) {
				// i番目の壁ポリゴンおｎアドレスを壁ポリゴンポインタ配列から取得
				poly = kabe[i];

				// ポリゴンとプレイヤーが当たっていなかったら次のカウントへ
				Vector3 capsuleStart = nowPos;
				Vector3 capsuleEnd = nowPos + Vector3(0.0f, polyOffset, 0.0f);

				if (HitCheck_Capsule_Triangle(
					Vector3::ToVECTOR(capsuleStart),
					Vector3::ToVECTOR(capsuleEnd),
					PLAYER_HIT_WIDTH,
					poly->Position[0],
					poly->Position[1],
					poly->Position[2]) == FALSE
					) continue;

				// ポリゴンに当たったフラグを立てる
				hitFlag = true;

				// 壁に当たったら壁に遮られない移動成分分だけ移動する
				{
					Vector3 slideVec;	// プレイヤーをスライドするベクトル
					// 進行方向ベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを算出
					slideVec = Vector3::Cross(MoveVec, Vector3::FromVECTOR(poly->Normal));

					// 元の移動成分から壁方向の移動成分を抜いたベクトル
					slideVec = Vector3::Cross(Vector3::FromVECTOR(poly->Normal), slideVec);

					// それを移動前のい座標に足したものを新たな座標とする
					nowPos = prevPos + slideVec;

				}

				// 新たな移動座標で壁ポリゴンと当たっていないかどうかを判定する
				for (j = 0; j < kabeNum; j++) {
					// j番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
					poly = kabe[j];

					// 当たっていたらループから抜ける
					Vector3 capsulePos = nowPos + Vector3(0.0f, polyOffset, 0.0f);
					if (HitCheck_Capsule_Triangle(
						Vector3::ToVECTOR(nowPos),
						Vector3::ToVECTOR(capsulePos),
						PLAYER_HIT_WIDTH,
						poly->Position[0],
						poly->Position[1],
						poly->Position[2]) == TRUE) break;
				}

				// 壁に当たったフラグをfalseにしてループから抜ける
				if (j == kabeNum) {
					hitFlag = false;
					break;
				}
			}
		}
		else {
			// 移動していない場合の処理

			// 壁のポリゴンの数だけ繰り返し
			for (i = 0; i < kabeNum; i++) {
				// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				poly = kabe[i];

				// ポリゴンに当たっていたら当たったフラグをtrueにしてループから抜ける
				Vector3 capsulePos = nowPos + Vector3(0.0f, polyOffset, 0.0f);
				if (HitCheck_Capsule_Triangle(
					Vector3::ToVECTOR(nowPos),
					Vector3::ToVECTOR(capsulePos),
					PLAYER_HIT_WIDTH,
					poly->Position[0],
					poly->Position[1],
					poly->Position[2]) == TRUE) {

					hitFlag = true;
					break;
				}
			}
		}

		// 壁に当たっていたら壁から押し出す処理を行う
		if (hitFlag) {
			// 壁からの押し出し処理を最大数繰り返す
			for (k = 0; k < HIT_TRYNUM; k++) {
				// 壁ポリゴンの数だけ繰り返し
				for (i = 0; i < kabeNum; i++) {
					// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
					poly = kabe[i];

					// プレイヤーと当たっているかを判定
					Vector3 capsulePos = nowPos + Vector3(0.0f, polyOffset, 0.0f);
					if (HitCheck_Capsule_Triangle(
						Vector3::ToVECTOR(nowPos),
						Vector3::ToVECTOR(capsulePos),
						PLAYER_HIT_WIDTH,
						poly->Position[0],
						poly->Position[1],
						poly->Position[2]) == FALSE) continue;

					// 当たっていたら基底距離分プレイヤーを壁の法線方向に移動させる
					Vector3 norm = Vector3::FromVECTOR(poly->Normal).Normalized();
					nowPos = nowPos + norm * HIT_SLIDE_LENGTH;

					// 移動したうえで壁ポリゴンと接触しているかどうかを判定
					for (j = 0; j < kabeNum; j++) {
						// 当たっていたらループを抜ける
						poly = kabe[j];
						Vector3 capsulePos = nowPos + Vector3(0.0f, polyOffset, 0.0f);
						if (HitCheck_Capsule_Triangle(
							Vector3::ToVECTOR(nowPos),
							Vector3::ToVECTOR(capsulePos),
							PLAYER_HIT_WIDTH,
							poly->Position[0],
							poly->Position[1],
							poly->Position[2]) == TRUE) break;
					}

					// すべてのポリ9権と当たっていなかったらここでループ終了
					if (j == kabeNum)break;
				}

				// i が kabeNumではない場合は全部のポリゴンで押し出しを試みる前に全ての壁ポリゴンと接触しなくなったということなのでループから抜ける
				if (i != kabeNum) break;

			}
		}
	}

	// 床との衝突
	if (yukaNum != 0) {
		float MaxY;
		bool HitFlag = false;

		// 一番高い床ポリゴンにぶつけるための判定用変数を初期化
		MaxY = 0.0f;

		// 床ポリゴンの数だけ繰り返し
		for (i = 0; i < yukaNum; i++) {
			// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
			poly = yuka[i];

			// 頭の先か少し下の位置までの当たり判定を行う
			Vector3 line1 = nowPos + Vector3(0.0f, polyOffset, 0.0f);
			Vector3 line2 = nowPos + Vector3(0.0f, -0.3f, 0.0f);
			lineRes = HitCheck_Line_Triangle(
				Vector3::ToVECTOR(line1),
				Vector3::ToVECTOR(line2),
				poly->Position[0],
				poly->Position[1],
				poly->Position[2]);

			// 当たっていなかったら何もしない
			if (lineRes.HitFlag == FALSE) continue;

			// すでに当たったポリゴンがあり、且つ今まで検出した床より低い場合はなにもしない
			if (hitFlag == true && MaxY > lineRes.Position.y) continue;

			// ポリゴンに当たったフラグを立てる
			hitFlag = true;

			// 接触したY座標を保存する
			MaxY = lineRes.Position.y;
		}

		// 床ポリゴンに当たったかどうか
		if (hitFlag == true) {
			// 当たった場合
			// 接触したポリゴンで一番高いY座標をプレイヤーのY座標にする
			nowPos.y = MaxY;

		}
		else {
			// 当たっていない場合

		}

		*position = nowPos;

		// モデルの周囲のポリゴンを解放
		MV1CollResultPolyDimTerminate(*hitDim);



	}


}