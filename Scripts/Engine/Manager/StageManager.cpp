/*
 *  @file StageManager.cpp
 *  @author oorui
 */

#include "StageManager.h"
#include <DxLib.h>
#include "../Load/Model/LoadModel.h"

 /*
  *  コンストラクタ
  */
StageManager::StageManager()
	: engine(nullptr)
	, pStage(nullptr) {
}

/*
 *  初期化
 */
void StageManager::Initialize(Engine& setEngine) {
	this->engine = &setEngine;
}

void StageManager::LoadStage(const int modelHandleBase) {
	pStage = std::make_unique<Stage>();
	pStage->ModelLoad(modelHandleBase);
}

/*
 *	ステージの当たり判定
 */
void StageManager::StageCollider(Vector3* position, Vector3 MoveVec) {
	if (!pStage)return;
	pStage->UpdateCollision(position, MoveVec);
}
/*
 *  更新
 */
void StageManager::Update() {
	if (!pStage)return;
	pStage->Update();

}

/*
 *  描画
 */
void StageManager::Render() {
	if (!pStage)return;
	pStage->Render();

}

/*
 *  終了
 */
void StageManager::Execute() {
	if (!pStage)return;
	pStage->Execute();
	pStage.reset();

}


/*
 *	ステージのFrameを取得
 */
int StageManager::GetStageFrame(const std::string& frameName) const {
	if (!pStage) return -1;

	// StageBase* から Stage* にキャスト
	Stage* stage = dynamic_cast<Stage*>(pStage.get());
	if (!stage) return -1;

	// ステージのモデルハンドルを取得
	int modelHandle = stage->GetModelHandle();
	return stage->GetFrameHandleByName(modelHandle, frameName);
}



/*
 *	スタート位置の取得
 */
Vector3 StageManager::GetStartPos() const {
	if (!pStage) return Vector3();

	Stage* stage = dynamic_cast<Stage*>(pStage.get());
	if (!stage) return Vector3();

	// ステージのモデルハンドルの取得
	int modelHandle = stage->GetModelHandle();

	// 開始位置の名前の取得
	std::string frameName = json["Player"]["StartPos"];
	// string型→const char* 型への型変換
	const char* cstr = frameName.c_str();

	// スタート位置のフレーム番号を取得
	int frameIndex = MV1SearchFrame(modelHandle, cstr);

	// スタート位置の座標を取得
	VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);

	// VECTOR型をVector3型に変換
	Vector3 startPos = FromVECTOR(framePos);

	// 座標を返す
	return startPos;
}


/*
 *	ゴール位置の取得
 */
Vector3 StageManager::GetGoalPos() const {
	if (!pStage)return Vector3();
	Stage* stage = dynamic_cast<Stage*>(pStage.get());
	if (!stage)return Vector3();

	// ステージのモデルハンドルの取得
	int modelHandle = stage->GetModelHandle();

	// 終了位置の名前の取得
	std::string framName = json["Player"]["GoalPos"];

	// string型→const char* 型への型変換
	const char* cstr = framName.c_str();

	// 終了位置のフレーム番号を取得
	int frameIndex = MV1SearchFrame(modelHandle, cstr);

	// 終了位置の座標を取得
	VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);

	// VECTOR型をVector3型に変換
	Vector3 goalPos = FromVECTOR(framePos);

	// 座標を返す
	return goalPos;

}

/*
 *	敵の初期生成位置の取得
 */
std::vector<Vector3> StageManager::GetEnemySpwanPos() const {
	std::vector<Vector3> result;
	if (!pStage)return result;

	Stage* stage = dynamic_cast<Stage*>(pStage.get());
	if (!stage)return result;

	// ステージのモデルハンドルの取得
	int modelHandle = stage->GetModelHandle();

	// 敵の生成位置の取得
	auto spawnArray = json["Enemy"]["SpwanPos"];

	// jsonファイルのテキストを配列で取得
	for (const auto& spawnName : spawnArray) {
		int frameIndex = MV1SearchFrame(modelHandle, spawnName.get<std::string>().c_str());
		if (frameIndex == -1) continue;

		VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);
		result.push_back(FromVECTOR(framePos));
	}

	return result;

}

/*
 * お宝の生成位置の取得
 */
std::vector<Vector3> StageManager::GetTreasureSpwanPos()const {
	std::vector<Vector3> result;
	if (!pStage)return result;

	Stage* stage = dynamic_cast<Stage*>(pStage.get());
	if (!stage)return result;

	// ステージのモデルハンドルの取得
	int modelHandle = stage->GetModelHandle();

	// お宝の生成位置の取得
	auto spawnArray = json["Treasure"]["SpwanPos"];

	// jsonファイルのテキストを配列で取得
	for (const auto& spawnName : spawnArray) {
		int frameIndex = MV1SearchFrame(modelHandle, spawnName.get<std::string>().c_str());
		if (frameIndex == -1)continue;

		VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);
		result.push_back(FromVECTOR(framePos));
	}

	return result;

}