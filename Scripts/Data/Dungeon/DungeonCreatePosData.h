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
	Vector3 position = V_ZERO;
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
	void Clear() {
		dungeonCreatePosMap.clear();
	}

	/*
	 *	@brief		JSONロード
	 *  @param[in]	json		読み込むJSONデータ
	 *  @param[in]	dungeonID	現在のダンジョンID
	 */
	void LoadFromJSON(const JSON& json, int dungeonID) {
		auto& data = dungeonCreatePosMap[dungeonID];

		LoadStartPos(json, data);		// 開始位置の設定
		LoadGoalPos(json, data);		// 出口位置の設定
		LoadEnemySpawn(json, data);		// 敵生成位置の設定
		LoadBossSpawn(json, data);		// ボス生成位置の設定
		LoadPointLight(json, data);		// ポイントライト生成位置の設定
		LoadStairs(json, data);			// 階段生成位置の設定
		LoadRespawn(json, data);		// リスポーン生成位置の設定
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
	void LoadEnemySpawn(const JSON& json, CreatePosData& data) {
		if (!json.contains(GameConst::_CREATE_POSNAME_ENEMY)) return;

		const auto& enemy = json[GameConst::_CREATE_POSNAME_ENEMY];

		if (!enemy.contains(GameConst::_CREATE_POSITION_SPAWN)) return;

		const auto& spawnRoot =
			enemy[GameConst::_CREATE_POSITION_SPAWN];

		for (auto it = spawnRoot.begin(); it != spawnRoot.end(); ++it) {
			int enemyID = std::stoi(it.key());

			if (!it.value().is_string()) continue;

			std::string frameName = it.value();

			data.enemy.position.emplace(
				enemyID,
				GetFramePosition(frameName)
			);
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

		const auto& player = json[GameConst::_CREATE_POSNAME_PLAYER];

		if (!player.contains("RespawnPos")) return;

		std::string frameName = player["RespawnPos"];

		data.respawn.position = GetFramePosition(frameName);
	}

public:

	/*
	 * 各データ取得
	 */

	const CreatePosData* GetCreatePosData(int dungeonID) const {
		auto it = dungeonCreatePosMap.find(dungeonID);

		if (it == dungeonCreatePosMap.end())
			return nullptr;

		return &it->second;
	}
};

#endif