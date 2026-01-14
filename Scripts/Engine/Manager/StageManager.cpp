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
	// ステージの実態を生成
	loadedStage = std::make_unique<Stage>();
}

/*
 *	ステージの読み込み
 */
void StageManager::LoadStage(const std::vector<int> modelHandleBase) {
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
	stageState.ResetStageModelHandle();
}

/*
 *	スタート位置の取得
 */
Vector3 StageManager::GetStartPos() const {
	if (!loadedStage)return Vector3();

	// ステージのモデルハンドルの取得
	int modelHandle = loadedStage->GetStageModelHandle();
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
std::vector<Vector3> StageManager::GetGoalPos() const {
	std::vector<Vector3> result;
	if (!loadedStage)return result;

	// ステージのモデルハンドルの取得
	int modelHandle = loadedStage->GetStageModelHandle();
	if (modelHandle == -1) return result;

	// 終了位置の名前の取得
	// jsonファイルの文字列がなかった場合のnullCheck
	if (
		!json.contains(GameConst::_CREATE_POSNAME_PLAYER) ||
		!json[GameConst::_CREATE_POSNAME_PLAYER].contains("GoalPos")
		)
		return result;

	// フレーム名を取得
	std::string framName = json[GameConst::_CREATE_POSNAME_PLAYER]["GoalPos"];

	// string型→const char* 型への型変換
	const char* cstr = framName.c_str();

	// 終了位置のフレーム番号を取得
	int frameIndex = MV1SearchFrame(modelHandle, cstr);
	if (frameIndex == -1)return result;

	// 終了位置の座標を取得
	VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);

	// VECTOR型をVector3型に変換
	Vector3 goalPos = FromVECTOR(framePos);
	result.push_back(FromVECTOR(framePos));
	// 座標を返す
	return result;

}

/*
 *	敵の初期生成位置の取得
 */
std::vector<Vector3> StageManager::GetEnemySpwanPos(std::vector<int>& id) const {
	std::vector<Vector3> result;

	if (!loadedStage)return result;

	// ステージのモデルハンドルの取得
	int modelHandle = loadedStage->GetStageModelHandle();
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
	for (int i = 0; i < spawnArray.size(); i++) {
		// 敵スポーンデータを格納する構造体
		EnemySpawnPoint eSpawnPos;
		eSpawnPos.id = i;

		// フレーム名を取得
		std::string frameName = spawnArray[i].get<std::string>();

		// フレームの番号を取得
		int frameIndex = MV1SearchFrame(modelHandle, frameName.c_str());
		if (frameIndex == -1) continue;

		// フレームの位置を取得
		VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);
		eSpawnPos.pos = FromVECTOR(framePos);
		result.push_back(FromVECTOR(framePos));
		id.push_back(eSpawnPos.id);
	}


	return result;

}

/*
 * ボスの生成位置の取得
 * @author	kuu
 */
std::vector<Vector3> StageManager::GetBossSpwanPos(std::vector<int>& id) const
{
	//　のちに実装
	return std::vector<Vector3>();
}

/*
 * お宝の生成位置の取得
 */
std::vector<Vector3> StageManager::GetTreasureSpwanPos()const {
	// 空の配列を作成
	std::vector<Vector3> result;

	if (!loadedStage)return result;

	// ステージのモデルハンドルの取得
	int modelHandle = loadedStage->GetStageModelHandle();
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

	if (!loadedStage)return result;

	// ステージのモデルハンドルの取得
	int modelHandle = loadedStage->GetStageModelHandle();
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
std::vector<Vector3> StageManager::GetStairsPos() const {
	std::vector<Vector3> result;
	if (!loadedStage)return result;

	// ステージのモデルハンドルの取得
	int modelHandle = loadedStage->GetStageModelHandle();
	if (modelHandle == -1) return result;

	// 階段位置の名前の取得
	// jsonファイルの文字列がなかった場合のnullCheck
	if (!json.contains(GameConst::_CREATE_POSNAME_PLAYER) || !json[GameConst::_CREATE_POSNAME_PLAYER].contains("StairsPos"))
		return result;
	std::string frameName = json[GameConst::_CREATE_POSNAME_PLAYER]["StairsPos"];

	// string型→const char* 型への型変換
	const char* cstr = frameName.c_str();

	// 階段位置のフレーム番号を取得
	int frameIndex = MV1SearchFrame(modelHandle, cstr);
	if (frameIndex == -1)return result;

	// 階段位置の座標を取得
	VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);

	// VECTOR型をVector3型に変換
	Vector3 stairsPos = FromVECTOR(framePos);
	result.push_back(FromVECTOR(framePos));
	// 座標を返す
	return result;

}

/*
 *	階段手前のリスポーン位置の取得
 */
Vector3 StageManager::GetRespawnPos() const {
	if (!loadedStage)return Vector3();

	// ステージモデルハンドルの取得
	int modelHandle = loadedStage->GetStageModelHandle();
	if (modelHandle == -1) return Vector3();

	// 開始位置の名前の取得
	// jsonファイルの文字列のなかった場合のnullCheck
	if (
		!json.contains(GameConst::_CREATE_POSNAME_PLAYER) ||
		!json[GameConst::_CREATE_POSNAME_PLAYER].contains("RespawnPos")
		)
		return Vector3();

	std::string frameName = json[GameConst::_CREATE_POSNAME_PLAYER]["RespawnPos"];
	// string型→const char* 型への型変換
	const char* cstr = frameName.c_str();

	// スタート位置のフレーム番号を取得
	int frameIndex = MV1SearchFrame(modelHandle, cstr);
	if (frameIndex == -1)return Vector3();

	// スタート位置の座標を取得
	VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);

	// VECTOR型をVector3型に変換
	Vector3 respawnPos = FromVECTOR(framePos);

	// 座標を返す
	return respawnPos;

}
