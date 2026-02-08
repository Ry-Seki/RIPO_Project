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
std::unordered_map<int, Vector3> StageManager::GetEnemySpwanPos() const {
	// 空の配列を作成
	std::unordered_map<int, Vector3> enemySpawnPosition;

	if (!loadedStage) return enemySpawnPosition;

	// ステージのモデルハンドルを取得
	const int modelHandle = loadedStage->GetStageModelHandle();
	if (modelHandle == -1) return enemySpawnPosition;

	// JSONの中を検索
	if (!json.contains(GameConst::_CREATE_POSNAME_ENEMY)) return enemySpawnPosition;
	if (!json[GameConst::_CREATE_POSNAME_ENEMY].contains("SpawnPos")) return enemySpawnPosition;

	// 敵の生成位置
	const auto& spawnObj = json[GameConst::_CREATE_POSNAME_ENEMY]["SpawnPos"];

	// 敵の生成位置のフレームを検索
	for (auto it = spawnObj.begin(); it != spawnObj.end(); ++it) {
		// 敵のIDをint型に変換して取得
		int enemyID = std::stoi(it.key());
		// フレーム名を取得
		const std::string frameName = it.value().get<std::string>();

		// モデルにフレーム名があるか検索
		int frameIndex = MV1SearchFrame(modelHandle, frameName.c_str());
		if (frameIndex == -1) continue;

		// フレームのワールド座標を取得
		VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);

		// Vector3に変換して配列に追加
		enemySpawnPosition.emplace(enemyID, FromVECTOR(framePos));
	}

	return enemySpawnPosition;

}

/*
 * ボスの生成位置の取得
 * @author	kuu
 */
std::vector<Vector3> StageManager::GetBossSpwanPos(std::vector<int>& id) const {
	//　のちに実装←後
	return std::vector<Vector3>();
}

/*
 * お宝の生成位置の取得
 */
std::unordered_map<int, Vector3> StageManager::GetTreasureSpawnPos(int setValue)const {
	// 空の配列を作成
	std::unordered_map<int, Vector3> treasureSpawnPositions;

	if (!loadedStage) return treasureSpawnPositions;

	// ステージのモデルハンドルを取得
	int modelHandle = loadedStage->GetStageModelHandle();
	if (modelHandle == -1) return treasureSpawnPositions;

	// jsonの中を検索
	if (!json.contains(GameConst::_CREATE_POSNAME_TREASURE)) return treasureSpawnPositions;
	if (!json[GameConst::_CREATE_POSNAME_TREASURE].contains(GameConst::_CREATE_POSITION_SPAWN)) return treasureSpawnPositions;

	const auto& spawnRoot =
		json[GameConst::_CREATE_POSNAME_TREASURE]
		[GameConst::_CREATE_POSITION_SPAWN];

	// 階層が存在するかどうか
	// 引数の階層IDを文字列に変換
	const std::string setKey = std::to_string(setValue);
	if (!spawnRoot.contains(setKey)) return treasureSpawnPositions;

	// お宝の生成位置を階層別に取得
	const auto& spawnObj = spawnRoot[setKey];


	// お宝毎のフレームを探す
	for (auto it = spawnObj.begin(); it != spawnObj.end(); ++it) {

		// お宝のIDをint型に変換して取得
		int treasureID = std::stoi(it.key());
		// フレーム名を取得
		const std::string frameName = it.value().get<std::string>();

		// モデルにフレーム名があるか検索
		int frameIndex = MV1SearchFrame(modelHandle, frameName.c_str());
		if (frameIndex == -1) continue;

		// フレームのワールド座標を取得
		VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);

		// Vector3に変換して配列に追加
		treasureSpawnPositions.emplace(treasureID, FromVECTOR(framePos));
	}

	return treasureSpawnPositions;

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
	// 空の配列を作成
	std::vector<Vector3> result;
	// ロードしたステージが無ければ、空の配列を返す
	if (!loadedStage)return result;

	// ステージのモデルハンドルの取得
	int modelHandle = loadedStage->GetStageModelHandle();
	if (modelHandle == -1) return result;

	// jsonから階段の位置を取得
	if (!json.contains(GameConst::_CREATE_POSNAME_PLAYER) ||
		!json[GameConst::_CREATE_POSNAME_PLAYER].contains("StairsPos")) {
		return result;
	}

	const auto& stairsJson = json[GameConst::_CREATE_POSNAME_PLAYER]["StairsPos"];
	std::vector<std::string>frameNames;

	if (stairsJson.is_string()) {
		frameNames.push_back(stairsJson.get<std::string>());
	}
	else if (stairsJson.is_array()) {
		for (const auto& elem : stairsJson) {
			if (elem.is_string()) {
				frameNames.push_back(elem.get<std::string>());
			}
		}
	}

	if (frameNames.empty()) return result;

	
	for (const auto& name : frameNames) {
		
		const char* cstr = name.c_str();

		
		int frameIndex = MV1SearchFrame(modelHandle, cstr);
		if (frameIndex == -1) continue;

		
		VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);

		
		result.push_back(FromVECTOR(framePos));
	}


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
	if (!json.contains(GameConst::_CREATE_POSNAME_PLAYER) ||
		!json[GameConst::_CREATE_POSNAME_PLAYER].contains("RespawnPos")) {
		return Vector3();
	}

	const auto& respawnJson = json[GameConst::_CREATE_POSNAME_PLAYER]["RespawnPos"];

	std::string frameName;

	
	if (respawnJson.is_string()) {
		frameName = respawnJson.get<std::string>();
	}
	
	else if (respawnJson.is_array() && !respawnJson.empty()) {
		
		if (respawnJson[0].is_string()) {
			frameName = respawnJson[0].get<std::string>();
		}
	}
	if (frameName.empty()) return Vector3();
	// string型→const char* 型への型変換
	const char* cstr = frameName.c_str();

	// スタート位置のフレーム番号を取得
	int frameIndex = MV1SearchFrame(modelHandle, cstr);
	if (frameIndex == -1) return Vector3();

	// スタート位置の座標を取得
	VECTOR framePos = MV1GetFramePosition(modelHandle, frameIndex);

	return FromVECTOR(framePos);

}
