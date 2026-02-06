/*
 *	@file	EnemyDataManager.h
 *	@author	kuu
 */
#ifndef _ENEMYDATAMANAGER_H_
#define _ENEMYDATAMANAGER_H_

#include "../Singleton.h"
#include "../Load/JSON/LoadJSON.h"
#include "../Load/LoadManager.h"
#include "../GameEnum.h"
#include <unordered_map>
#include <string>

class EnemyDataManager : public Singleton<EnemyDataManager> {
	friend class Singleton<EnemyDataManager>;
private:
	struct EnemyStatus {
		int ID;		// 識別ID
		int HP;		// 最大HP
		int attack;	// 攻撃力
		int bounty;	// 討伐金
	};
	// すべてのエネミーのステータスデータ
	std::unordered_map<int, EnemyStatus> enemyStatus;

	// jsonデータのファイルパス
	const std::string ENEMY_DATA_PATH = "Data/Enemy/EnemyStatusData.json";
	// 敵の種類
	const std::string TUTORIAL_ENEMY = "TutorialEnemy";
	const std::string STAGE1_ENEMY = "Stage1Enemy";
	const std::string STAGE2_ENEMY = "Stage2Enemy";
	const std::string STAGE3_ENEMY = "Stage3Enemy";
	const std::string STAGE4_ENEMY = "Stage4Enemy";
	const std::string STAGE1_BOSS = "Stage1Boss";
	const std::string STAGE2_BOSS = "Stage2Boss";
	const std::string STAGE3_BOSS = "Stage3Boss";
	const std::string STAGE4_BOSS = "Stage4Boss";
	// 敵のステータス
	const std::string ID = "ID";
	const std::string HP = "HP";
	const std::string ATTACK = "Attack";
	const std::string BOUNTY = "Bounty";
	// enum指定用map
	std::unordered_map<GameEnum::EnemyType, std::string> enemiesName;

private:
	EnemyDataManager() = default;
	~EnemyDataManager() = default;

private:
	/*
	 *	jsonデータをマップに登録
	 */
	void SetMapJsonData(const JSON& setData) {
		for (int i = 0; i < static_cast<int>(GameEnum::EnemyType::Max); i++) {
			auto enemyName = enemiesName[static_cast<GameEnum::EnemyType>(i)];

			enemyStatus[i].ID = setData[enemyName][ID].get<int>();
			enemyStatus[i].HP = setData[enemyName][HP].get<int>();
			enemyStatus[i].attack = setData[enemyName][ATTACK].get<int>();
			enemyStatus[i].bounty = setData[enemyName][BOUNTY].get<int>();
		}
	}

public:
	/*
	 *	初期化
	 */
	void Initialize() {
		enemiesName[GameEnum::EnemyType::TutorialEnemy] = TUTORIAL_ENEMY;
		enemiesName[GameEnum::EnemyType::Stage1Enemy] = STAGE1_ENEMY;
		enemiesName[GameEnum::EnemyType::Stage2Enemy] = STAGE2_ENEMY;
		enemiesName[GameEnum::EnemyType::Stage3Enemy] = STAGE3_ENEMY;
		enemiesName[GameEnum::EnemyType::Stage4Enemy] = STAGE4_ENEMY;
		enemiesName[GameEnum::EnemyType::Stage1Boss] = STAGE1_BOSS;
		enemiesName[GameEnum::EnemyType::Stage2Boss] = STAGE2_BOSS;
		enemiesName[GameEnum::EnemyType::Stage3Boss] = STAGE3_BOSS;
		enemiesName[GameEnum::EnemyType::Stage4Boss] = STAGE4_BOSS;
		auto json = LoadManager::GetInstance().LoadResource<LoadJSON>(ENEMY_DATA_PATH);
		LoadManager::GetInstance().SetOnComplete([this, json]() {
			SetMapJsonData(json->GetData());
		});
	}

public:
	/*
	 *	各敵のデータ取得
	 */
	inline EnemyStatus GetWeaponData(GameEnum::EnemyType enemy) {
		return enemyStatus[static_cast<int>(enemy)];
	}

};

#endif // !_ENEMYDATAMANAGER_H_