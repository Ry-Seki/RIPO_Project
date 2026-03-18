/*
 *  @file DungeonCreatePosData.h
 */

#ifndef _DUNGEONCREATEPOSDATA_H_
#define _DUNGEONCREATEPOSDATA_H_

#include "../../Engine/JSON.h"
#include "../../Engine/VecMath.h"
#include "../../Engine/GameConst.h"

#include <unordered_map>
#include <vector>
#include <string>

 /*
  *  @brief  開始位置データ
  */
struct StartPosData {
	Vector3 position = V_ZERO;
};
/*
 *  @brief  出口位置データ
 */
struct GoalPosData {
	std::vector<Vector3> position;
};
/*
 *  @brief  敵スポーン位置データ
 */
struct EnemySpawnPosData {
	std::unordered_map<int, Vector3> position;
};
/*
 *  @brief  ボススポーン位置データ
 */
struct BossSpawnPosData {
	std::unordered_map<int, Vector3> position;
};
/*
 *  @brief  お宝生成位置データ
 */
struct TreasurePosData {
	std::unordered_map<int, Vector3> position;
};
/*
 *  @brief  ポイントライト配置位置データ
 */
struct PointLightPosData {
	std::vector<Vector3> position;
};
/*
 *  @brief  階段生成位置データ
 */
struct StairsPosData {
	std::vector<Vector3> position;
};
/*
 *  @brief  リスポーン生成位置データ
 */
struct RespawnPosData {
	std::vector<Vector3> position;
};

/*
 *  1ダンジョン分の生成情報
 */
struct CreatePosData {

	StartPosData start;         // 開始位置
	GoalPosData goal;           // 出口位置
	EnemySpawnPosData enemy;    // 敵位置
	BossSpawnPosData boss;      // ボス位置
	TreasurePosData treasure;   // お宝位置
	PointLightPosData light;    // 光源位置
	StairsPosData stair;        // 階段位置
	RespawnPosData respawn;     // 再出現位置
};

/*
 *  ダンジョンの生成位置を取得
 */
class DungeonCreatePosData {
private:
	int modelHandle = -1;
	std::unordered_map<int, CreatePosData> dungeonCreatePosMap;

private:

	/*
	 * フレーム名からワールド座標を取得
	 */
	Vector3 GetFramePosition(const std::string& frameName) const {
		int frameIndex = MV1SearchFrame(modelHandle, frameName.c_str());
		if (frameIndex == -1) return V_ZERO;

		VECTOR pos = MV1GetFramePosition(modelHandle, frameIndex);

		// VECTOR → Vector3変換
		return FromVECTOR(pos);
	}

public:
	/*
	 *  @brief  コンストラクタ
	 */
	DungeonCreatePosData() = default;
	/*
	 *  @brief  デストラクタ
	 */
	~DungeonCreatePosData() = default;

	/*
	 *  @brief  モデルハンドル設定
	 *  @param[in]  ダンジョンのモデルハンドル
	 */
	void SetModelHandle(int handle) { modelHandle = handle; }

	/*
	 *	@brief	データクリア
	 */
	void ClearCreatePosData() {
		dungeonCreatePosMap.clear();
	}

	/*
	 *	@brief		JSONロード
	 *  @param[in]	json		読み込むJSONデータ
	 *  @param[in]	dungeonID	現在のダンジョンID
	 */
	void LoadFromJSON(const JSON& json, int dungeonID, int floorID) {
		dungeonCreatePosMap.clear();
		auto& data = dungeonCreatePosMap[dungeonID];

		LoadStartPos(json, data);			// 開始位置の設定
		LoadGoalPos(json, data);			// 出口位置の設定
		LoadEnemySpawn(json, data,floorID);	// 敵生成位置の設定
		LoadBossSpawn(json, data);			// ボス生成位置の設定
		LoadPointLight(json, data);			// ポイントライト生成位置の設定
		LoadStairs(json, data);				// 階段生成位置の設定
		LoadRespawn(json, data);			// リスポーン生成位置の設定
		LoadTreasure(json, data, floorID);	// お宝生成位置の設定
	}

private:

	/*
	 * スタート位置
	 */
	void LoadStartPos(const JSON& json, CreatePosData& data) {
		if (!json.contains(GameConst::_CREATE_POSNAME_PLAYER)) return;

		const auto& player = json[GameConst::_CREATE_POSNAME_PLAYER];

		if (!player.contains(GameConst::_CREATE_POSITION_STARTPOS)) return;

		std::string frameName =
			player[GameConst::_CREATE_POSITION_STARTPOS];

		data.start.position = GetFramePosition(frameName);
	}

	/*
	 * ゴール位置
	 */
	void LoadGoalPos(const JSON& json, CreatePosData& data) {
		if (!json.contains(GameConst::_CREATE_POSNAME_PLAYER)) return;

		const auto& player = json[GameConst::_CREATE_POSNAME_PLAYER];

		if (!player.contains(GameConst::_CREATE_POSITION_GOALPOS)) return;

		const auto& goalJson =
			player[GameConst::_CREATE_POSITION_GOALPOS];

		if (goalJson.is_string()) {
			data.goal.position.push_back(
				GetFramePosition(goalJson.get<std::string>())
			);
		}
		else if (goalJson.is_array()) {
			for (const auto& elem : goalJson) {
				if (!elem.is_string()) continue;

				data.goal.position.push_back(
					GetFramePosition(elem.get<std::string>())
				);
			}
		}
	}

	/*
	 * 敵生成位置
	 */
	void LoadEnemySpawn(const JSON& json, CreatePosData& data, int floorID) {
		// Enemyノードが無ければ終了
		if (!json.contains(GameConst::_CREATE_POSNAME_ENEMY)) return;

		const auto& enemy = json[GameConst::_CREATE_POSNAME_ENEMY];

		// SpawnPosが無ければ終了
		if (!enemy.contains(GameConst::_CREATE_POSITION_SPAWN)) return;

		const auto& spawnRoot = enemy[GameConst::_CREATE_POSITION_SPAWN];

		// floorIDを文字列に変換
		const std::string floorKey = std::to_string(floorID);

		// 該当階層が無ければ終了
		if (!spawnRoot.contains(floorKey)) return;

		const auto& enemyObj = spawnRoot[floorKey];

		// 敵IDごとのフレーム名を取得
		for (auto it = enemyObj.begin(); it != enemyObj.end(); ++it) {

			// enemyID取得
			int enemyID = std::stoi(it.key());

			if (!it.value().is_string()) continue;

			// フレーム名取得
			std::string frameName = it.value().get<std::string>();

			// フレーム座標取得
			Vector3 pos = GetFramePosition(frameName);

			// mapに追加
			data.enemy.position.emplace(enemyID, pos);
		}
	}

	/*
	 * ボス生成位置
	 */
	void LoadBossSpawn(const JSON& json, CreatePosData& data) {
		if (!json.contains(GameConst::_CREATE_POSNAME_ENEMY)) return;

		const auto& enemy = json[GameConst::_CREATE_POSNAME_ENEMY];

		if (!enemy.contains(GameConst::_CREATE_POSITION_BOSSSPAWN)) return;

		const auto& spawnRoot =
			enemy[GameConst::_CREATE_POSITION_BOSSSPAWN];

		for (auto it = spawnRoot.begin(); it != spawnRoot.end(); ++it) {
			int bossID = std::stoi(it.key());

			if (!it.value().is_string()) continue;

			std::string frameName = it.value();

			data.boss.position.emplace(
				bossID,
				GetFramePosition(frameName)
			);
		}
	}

	/*
	 * ポイントライト
	 */
	void LoadPointLight(const JSON& json, CreatePosData& data) {
		if (!json.contains(GameConst::_CREATE_POSNAME_LIGHT)) return;

		const auto& light = json[GameConst::_CREATE_POSNAME_LIGHT];

		if (!light.contains(GameConst::_CREATE_POSITION_LIGHTPOS)) return;

		const auto& arr =
			light[GameConst::_CREATE_POSITION_LIGHTPOS];

		for (const auto& elem : arr) {
			if (!elem.is_string()) continue;

			data.light.position.push_back(
				GetFramePosition(elem.get<std::string>())
			);
		}
	}

	/*
	 * 階段
	 */
	void LoadStairs(const JSON& json, CreatePosData& data) {
		if (!json.contains(GameConst::_CREATE_POSNAME_PLAYER)) return;

		const auto& player = json[GameConst::_CREATE_POSNAME_PLAYER];

		if (!player.contains(GameConst::_CREATE_POSITION_STAIRPOS)) return;

		const auto& arr =
			player[GameConst::_CREATE_POSITION_STAIRPOS];

		for (const auto& elem : arr) {
			if (!elem.is_string()) continue;

			data.stair.position.push_back(
				GetFramePosition(elem.get<std::string>())
			);
		}
	}

	/*
	 * リスポーン
	 */
	void LoadRespawn(const JSON& json, CreatePosData& data) {
		if (!json.contains(GameConst::_CREATE_POSNAME_PLAYER)) return;

		// 
		const auto& player = json[GameConst::_CREATE_POSNAME_PLAYER];
		if (!player.contains(GameConst::_CREATE_POSITION_RESPAWNPOS)) return;

		const auto& arr = player[GameConst::_CREATE_POSITION_RESPAWNPOS];

		for (const auto& elem : arr) {
			if (!elem.is_string())continue;
			data.respawn.position.push_back(GetFramePosition(elem.get<std::string>()));
		}
	}

	/*
	 *	@brief		お宝位置取得
	 *	@param[in]	json	読み込むjsonデータ
	 *  @param[in]	data	設定先のデータ
	 *	@param[in]	flooriD	階層のID
	 */
	void LoadTreasure(const JSON& json, CreatePosData& data, int floorID) {
		if (!json.contains(GameConst::_CREATE_POSNAME_TREASURE))return;

		const auto& treasure = json[GameConst::_CREATE_POSNAME_TREASURE];
		if (!treasure.contains(GameConst::_CREATE_POSITION_SPAWN))return;
		// ルートを設定
		const auto& spawnRoot = json[GameConst::_CREATE_POSNAME_TREASURE][GameConst::_CREATE_POSITION_SPAWN];

		// 階層が存在するかどうか
		// 引数の階層IDを文字列に変換
		const std::string setKey = std::to_string(floorID);
		if (!spawnRoot.contains(setKey)) return;
		// お宝の生成位置を階層別に取得
		const auto& spawnObj = spawnRoot[setKey];


		// お宝毎のフレームを探す
		for (auto it = spawnObj.begin(); it != spawnObj.end(); ++it) {

			// お宝のIDをint型に変換して取得
			int treasureID = std::stoi(it.key());
			// フレーム名を取得
			const std::string frameName = it.value().get<std::string>();

			// データを追加
			data.treasure.position.emplace(
				treasureID,
				GetFramePosition(frameName)
			);

		}
	}

public:

	/*
	 * 各データ取得
	 */

	bool GetCreatePosData(int dungeonID, CreatePosData& data) {
		auto it = dungeonCreatePosMap.find(dungeonID);

		if (it == dungeonCreatePosMap.end())
			return false;

		data = it->second;

		return true;
	}

	// データ構造体の取得

};

#endif