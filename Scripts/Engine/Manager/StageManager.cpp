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

/*
 *  ステージの読み込み
 */
void StageManager::LoadStage(const std::string& stageCsvPath) {

	auto StageModel = std::make_shared<LoadModel>(stageCsvPath);
	LoadManager::GetInstance().AddLoader(StageModel);

	pStage = std::make_unique<Stage>();
	pStage->Load(StageModel);
}

/*
 *	ステージの当たり判定
 */
void StageManager::StageCollider(Vector3* position, Vector3 PolyPos1, Vector3 PloyPos2, Vector3 MoveVec) {
	if (!pStage)return;
	pStage->UpdateCollision(position, PolyPos1, PloyPos2, MoveVec);
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