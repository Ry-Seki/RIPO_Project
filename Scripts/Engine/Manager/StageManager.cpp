/*
 *  @file StageManager.cpp
 *  @author oorui
 */

#include "StageManager.h"
#include <DxLib.h>
#include "../Load/Model/LoadModel.h"
#include "../GameConst.h"
#include "../Stage/StageMemoryProfiler.h"
#include "../StringUtility.h"
 /*
  *  コンストラクタ
  */
StageManager::StageManager()
	: engine(nullptr) 
	, lightDirection(0.2f, -20.0f, 0.3f)	// 光源の向き
	, pointLightColor(1.0f, 0.8f, 0.6f)		// ポイントライトの色
	, pointLightRange(10000.0f)				// ポイントライトの範囲
{
}

/*
 *  初期化
 */
void StageManager::Initialize(Engine& setEngine) {
	this->engine = &setEngine;
	// ステージの実態を生成
	loadedStage = std::make_unique<Stage>();
}

/*
 *	ステージの読み込み
 */
void StageManager::LoadStage(const std::vector<int> modelHandleBase) {

#if _DEBUG
	StageMemoryProfiler::Initialize("Data/Dungeon/DungeonMemory/DungeonMemoryProfiler.csv");
	StageMemoryProfiler::Log("ステージ読み込み開始");
#endif

	// モデルハンドルの追加、複製
	for (auto model : modelHandleBase) {
		// モデルハンドルを複製
		int duplicatedModel = MV1DuplicateModel(model);
		// ステージのモデルハンドルを追加
		stageState.AddStageModelHandle(duplicatedModel);
	}

	// 初期ステージ設定
	if (loadedStage && stageState.GetCurrentStageHandle() == -1 && stageState.GetStageCount() > 0) {
		// 指定の番号のモデルハンドルを取得
		int firstHandle = stageState.GetStageModelHandleAt(0);
		if (firstHandle != -1) {
			loadedStage->SetModelHandle(firstHandle);
			// 最初に入れられたステージを初期ステージに設定
			stageState.SetCurrentStageIndex(0);
		}
	}

	
}

/*
 *	現在のインデックスのステージに切り替える
 */
void StageManager::ChangeStage() {
	int currentHandle = stageState.GetCurrentStageHandle();
	if (currentHandle == -1)return;


	loadedStage->SetModelHandle(currentHandle);
	
}

/*
 * @brief 次のステージへ進む
 */
void StageManager::NextStage(int setID) {
	stageState.NextStage(setID);
	ChangeStage();
}

/*
 * @brief 前のステージに戻る
 */
void StageManager::PrevStage() {
	stageState.PrevStage();
	ChangeStage();
}

/*
 *	ステージの当たり判定
 */
void StageManager::StageCollider(GameObject* other, Vector3 MoveVec) {
	WithCurrentStage([&](StageBase& stage) { stage.UpdateCollision(other, MoveVec); });
#if _DEBUG
	StageMemoryProfiler::UpdatePeak();
#endif
}
/*
 *  更新
 */
void StageManager::Update() {

	WithCurrentStage([](StageBase& stage) { stage.Update(); });
}

/*
 *  描画
 */
void StageManager::Render() {
	WithCurrentStage([](StageBase& stage) { stage.Render(); });
	LightSettings();
}


/*
 *  終了
 */
void StageManager::Execute() {
#if _DEBUG
	StageMemoryProfiler::Log("ゲーム終了");
	StageMemoryProfiler::LogPeak(); // 最大使用量
	StageMemoryProfiler::Execute();	// 計測終了
#endif
	stageState.ResetStageModelHandle();
}

/*
 *	ステージライトの設定
 */
void StageManager::LightSettings() {
	// 古いライトを削除
	DeleteLightHandleAll();

	// マップ全体のライト設定
	// アンビエントカラーの設定
	SetLightAmbColor(GameConst::_MAP_AMB_COLOR);
	// ライトの方向を設定する
	SetLightDirection(ToVECTOR(lightDirection));
	SetLightDifColor(GameConst::_MAP_DIF_COLOR);
	SetLightSpcColor(GameConst::_MAP_SPC_COLOR);

	// ポイントライトがおける場所がある分、ポイントライトを配置する
	for (const auto& pos : pointLightPos) {
		// ポイントライトのハンドルを作成
		int pLight = CreatePointLightHandle(ToVECTOR(pos), pointLightRange, _POINT_ATTAN.Atten0, _POINT_ATTAN.Atten1, _POINT_ATTAN.Atten2);
		// ポイントライトの色を指定する
		SetLightDifColorHandle(pLight, GetColorF(pointLightColor.x, pointLightColor.y, pointLightColor.z, Vector3::one.x));
		SetLightSpcColorHandle(pLight, GameConst::_POINT_SPC_COLOR);
		SetLightEnableHandle(pLight, TRUE);
	}
}
