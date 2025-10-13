/*
 *  @file StageManager.cpp
 *  @author oorui
 */

#include "StageManager.h"
#include <DxLib.h>

 /*
  *  コンストラクタ
  */
StageManager::StageManager()
	: pEngine(nullptr)
	, pStage(nullptr) {
}

/*
 *  初期化
 */
void StageManager::Initialize(Engine* pEngine) {
	this->pEngine = pEngine;
}

/*
 *  ステージの読み込み
 */
void StageManager::LoadStage(const std::string& stageCsvPath) {
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