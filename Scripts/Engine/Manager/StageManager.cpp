/*
 *  @file StageManager.cpp
 *  @author oorui
 */

#include "StageManager.h"
#include <DxLib.h>
#include "../Load/LoadModel.h"

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

	auto StageModel = std::make_shared<LoadModel>("Res/Model/Stage/StageModel_1.mv1");
	LoadManager::GetInstance().AddLoader(StageModel);

	pStage = std::make_unique<Stage>();
	pStage->Load(stageCsvPath);
}

/*
 *  更新
 */
void StageManager::Update() {
	if (pStage) {
		pStage->Update();
	}
}

/*
 *  描画
 */
void StageManager::Render() {
	if (pStage) {
		pStage->Render();
	}
}

/*
 *  終了
 */
void StageManager::Execute() {
	if (pStage) {
		pStage->Execute();
		pStage.reset();
	}
}