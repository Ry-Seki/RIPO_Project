/*
 *  @file Stage.cpp
 *  @author oorui
 */

#include "Stage.h"
#include "StageCollision.h"                


#include "../Manager/StageManager.h"         
#include "../GameConst.h"                    
#include "../GameObject.h"                   
#include "../Component/CapsuleCollider.h"    
#include "../Component/GravityComponent.h"   
#include "../Component/Character/CharacterUtility.h"
#include "StageMemoryProfiler.h"

using namespace CharacterUtility;

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
 *	ステージデータの読み込み
 *  @pram const int modelHandle
 */
void Stage::SetModelHandle(const int modelHandleBase) {
	modelHandle = modelHandleBase;

	// 当たり判定クラスの生成
	collision = std::make_unique<StageCollision>(modelHandle);

#if _DEBUG
	// メモリログに書き込む
	StageMemoryProfiler::Log("当たり判定生成後");
#endif
}

/*
 *	更新
 */
void Stage::Update() {
}

/*
 *  描画
 */
void Stage::Render() {
	if (modelHandle >= 0) {
		// モデルの描画
		MV1DrawModel(modelHandle);

#if _DEBUG
		// プレイヤーの取得
		player = GetPlayer();
		if (player != nullptr) {
			// プレイヤーの直前の位置を計算
			Vector3 prevPos = player->GetComponent<PlayerComponent>()->GetOwner()->position - player->GetComponent<PlayerComponent>()->GetMoveVec();
			// ステージ当たり判定描画
			collision->StageColliderRenderer(player.get(), player->GetComponent<PlayerComponent>()->GetMoveVec(), prevPos);
			//collision->StageColliderGridRenderer(player.get(), player->GetComponent<PlayerComponent>()->GetMoveVec(), prevPos);
			collision->ClearGrid();
			collision->DrawGrid(player.get());

		}
#endif // _DEBUG


	}
}

/*
 *	終了処理
 */
void Stage::Execute() {
#if _DEBUG
	StageMemoryProfiler::Log("ステージ破棄前");
#endif
	if (modelHandle >= 0) {
		// モデルを非表示
		Clean(modelHandle);
		// モデルの片付け
		MV1DeleteModel(modelHandle);
		// モデルをnull
		modelHandle = -1;
	}

#if _DEBUG
	StageMemoryProfiler::Log("ステージ破棄後");
#endif
}

/*
 * @brief ステージの当たり判定を更新
 * @param position     オブジェクト位置	参照渡しで座標変更
 * @param MoveVec      移動ベクトル
 */
void Stage::UpdateCollision(GameObject* other, Vector3 MoveVec) {
	if (!collision)return;

	// ステージの当たり判定を実行
	collision->UpdateCollision(other, MoveVec);

}


