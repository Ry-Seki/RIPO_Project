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
#include "../Component/Character/CharacterUtility.h"

using namespace CharacterUtility;

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
		// プレイヤーの取得
		player = GetPlayer();
		if (player != nullptr) {
			// ステージの当たり判定の描画
			StageColliderRenderer(player.get(), player->GetComponent<PlayerComponent>()->GetMoveVec());

		}



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


