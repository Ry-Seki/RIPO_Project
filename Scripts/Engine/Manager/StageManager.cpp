/*
 *  @file StageManager.cpp
 *  @author oorui
 */

#include "StageManager.h"
#include <DxLib.h>
#include "../Load/Model/LoadModel.h"
#include "../GameConst.h"

 /*
  *  コンストラクタ
  */
StageManager::StageManager()
	: engine(nullptr) {
}

/*
 *  初期化
 */
void StageManager::Initialize(Engine& setEngine) {
	this->engine = &setEngine;
}

/*
 *	ステージの読み込み
 */
void StageManager::LoadStage(const int modelHandleBase) {

	loadedStage = std::make_unique<Stage>();
	loadedStage->SetModelHandle(modelHandleBase);
	ChangeStage();
}

/*
 *	ステージの切り替え
 */
void StageManager::ChangeStage() {
	if (!loadedStage)return;
	stageState.SetCurrentStage(std::move(loadedStage));
}

/*
 *	前のステージに戻す
 */
void StageManager::RestorePrevStage() {
	stageState.RestorePreviousStage();
}

/*
 *	ステージの当たり判定
 */
void StageManager::StageCollider(Vector3* position, Vector3 MoveVec) {
	WithCurrentStage([&](StageBase& stage) { stage.UpdateCollision(position, MoveVec); });
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
}


/*
 *  終了
 */
void StageManager::Execute() {
	auto* currentStage = stageState.GetCurrentStage();
	auto* prevStage = stageState.GetPrevStage();

	if (currentStage) currentStage->Execute();
	if (prevStage) prevStage->Execute();
}


/*
 *	ステージのFrameを取得
 */
int StageManager::GetStageFrame(const std::string& frameName) const {
	auto* currentStage = stageState.GetCurrentStage();
	if (!currentStage) return -1;

	// StageBase* から Stage* にキャスト
	Stage* stage = dynamic_cast<Stage*>(currentStage);
	if (!stage) return -1;
	if (stage->GetModelHandle() < 0) return -1;

	// ステージのモデルハンドルを取得
	int modelHandle = stage->GetModelHandle();
	if (modelHandle == -1)return -1;

	// ステージモデルのフレームを返す
	return stage->GetFrameHandleByName(modelHandle, frameName);
}



/*
 *	スタート位置の取得
 */
Vector3 StageManager::GetStartPos() const {
	auto* currentStage = stageState.GetCurrentStage();
	if (!currentStage) return Vector3();

	Stage* stage = dynamic_cast<Stage*>(currentStage);
	if (!stage) return Vector3();

	// ステージのモデルハンドルの取得
	int modelHandle = stage->GetModelHandle();
	if (modelHandle == -1) return Vector3();

	// 開始位置の名前の取得
	// jsonファイルの文字列がなかった場合のnullCheck
	if (
		!json.contains(GameConst::_CREATE_POSNAME_PLAYER) ||
		!json[GameConst::_CREATE_POSNAME_PLAYER].contains("StartPos")
		)
		return Vector3();

	std::string frameName = json[GameConst::_CREATE_POSNAME_PLAYER]["StartPos"];
	// string型→const char* 型への型変換
	const char* cstr = frameName.c_str();

	// スタート位置のフレーム番号を取得
	int frameIndex = MV1SearchFrame(modelHandle, cstr);
	if (frameIndex == -1)return Vector3();

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
	// 現在のステージを取得
	auto* currentStage = stageState.GetCurrentStage();
	if (!currentStage) return Vector3();

	Stage* stage = dynamic_cast<Stage*>(currentStage);
	if (!stage)return Vector3();

	// ステージのモデルハンドルの取得
	int modelHandle = stage->GetModelHandle();
	if (modelHandle == -1) return Vector3();

	// 終了位置の名前の取得
	// jsonファイルの文字列がなかった場合のnullCheck
	if (
		!json.contains(GameConst::_CREATE_POSNAME_PLAYER) ||
		!json[GameConst::_CREATE_POSNAME_PLAYER].contains("GoalPos")
		)
		return Vector3();

	// フレーム名を取得
	std::string framName = json[GameConst::_CREATE_POSNAME_PLAYER]["GoalPos"];

	// string型→const char* 型への型変換
	const char* cstr = framName.c_str();

	// 終了位置のフレーム番号を取得
	int frameIndex = MV1SearchFrame(modelHandle, cstr);
	if (frameIndex == -1)return Vector3();

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
	auto* currentStage = stageState.GetCurrentStage();
	if (!currentStage) return result;


	Stage* stage = dynamic_cast<Stage*>(currentStage);
	if (!stage)return result;

	// ステージのモデルハンドルの取得
	int modelHandle = stage->GetModelHandle();
	if (modelHandle == -1) return result;

	// 敵の生成位置の取得
	// jsonファイルの文字列がなかった場合のnullCheck
	if (
		!json.contains(GameConst::_CREATE_POSNAME_ENEMY) ||
		!json[GameConst::_CREATE_POSNAME_ENEMY].contains("SpawnPos")
		)
		return result;

	// フレーム名の取得
	auto spawnArray = json[GameConst::_CREATE_POSNAME_ENEMY]["SpawnPos"];

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
	// 空の配列を作成
	std::vector<Vector3> result;
	// 現在のステージの取得
	auto* currentStage = stageState.GetCurrentStage();
	// 現在のステージが取得できなければ、空の配列を返す
	if (!currentStage) return result;

	Stage* stage = dynamic_cast<Stage*>(currentStage);
	if (!stage)return result;

	// ステージのモデルハンドルの取得
	int modelHandle = stage->GetModelHandle();
	// モデルハンドルが空だったら、空の配列を返す
	if (modelHandle == -1) return result;

	// お宝の生成位置の取得
	// jsonファイルの文字列がなかった場合のnullCheck
	if (!json.contains(GameConst::_CREATE_POSNAME_TREASURE) || !json[GameConst::_CREATE_POSNAME_TREASURE].contains(GameConst::_CREATE_POSITION_SPAWN))
		return result;
	auto spawnArray = json[GameConst::_CREATE_POSNAME_TREASURE][GameConst::_CREATE_POSITION_SPAWN];

	// jsonファイルのテキストを配列で取得
	for (const auto& spawnName : spawnArray) {
		int frameIndex = MV1SearchFrame(modelHandle, spawnName.get<std::string>().c_str());
		if (frameIndex == -1)continue;

		VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);
		result.push_back(FromVECTOR(framePos));
	}

	return result;

}

/*
 *	ポイントライト生成位置の取得
 */
std::vector<Vector3> StageManager::GetPointLightPos()const {
	std::vector<Vector3> result;
	auto* currentStage = stageState.GetCurrentStage();
	if (!currentStage) return result;


	Stage* stage = dynamic_cast<Stage*>(currentStage);
	if (!stage)return result;

	// ステージのモデルハンドルの取得
	int modelHandle = stage->GetModelHandle();
	if (modelHandle == -1) return result;

	// jsonから座標を取得
	if (json.contains(GameConst::_CREATE_POSNAME_LIGHT) && json[GameConst::_CREATE_POSNAME_LIGHT].contains("LightPos")) {
		auto spawnArray = json[GameConst::_CREATE_POSNAME_LIGHT]["LightPos"];
		for (const auto& frameNameJson : spawnArray) {
			std::string frameName = frameNameJson.get<std::string>();
			int frameIndex = MV1SearchFrame(modelHandle, frameName.c_str());
			if (frameIndex == -1) continue;

			VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);
			result.push_back(FromVECTOR(framePos));
		}
	}

	return result;

}

/*
 *	階層移動用階段位置の取得
 */
Vector3 StageManager::GetStairsPos() const {
	auto* currentStage = stageState.GetCurrentStage();
	if (!currentStage)return Vector3();

	Stage* stage = dynamic_cast<Stage*>(currentStage);
	if (!stage)return Vector3();

	// ステージのモデルハンドルの取得
	int modelHandle = stage->GetModelHandle();
	if (modelHandle == -1)return Vector3();

	// 階段位置の名前の取得
	// jsonファイルの文字列がなかった場合のnullCheck
	if (!json.contains(GameConst::_CREATE_POSNAME_PLAYER) || !json[GameConst::_CREATE_POSNAME_PLAYER].contains("StairsPos"))
		return Vector3();
	std::string frameName = json[GameConst::_CREATE_POSNAME_PLAYER]["StairsPos"];

	// string型→const char* 型への型変換
	const char* cstr = frameName.c_str();

	// 階段位置のフレーム番号を取得
	int frameIndex = MV1SearchFrame(modelHandle, cstr);
	if (frameIndex == -1)return Vector3();

	// 階段位置の座標を取得
	VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);

	// VECTOR型をVector3型に変換
	Vector3 stairsPos = FromVECTOR(framePos);

	// 座標を返す
	return stairsPos;

}
