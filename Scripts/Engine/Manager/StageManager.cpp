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