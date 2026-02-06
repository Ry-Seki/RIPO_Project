/*
 *	@file	EnemyDataManager.h
 *	@author	kuu
 */
#ifndef _ENEMYDATAMANAGER_H_
#define _ENEMYDATAMANAGER_H_

#include "../Singleton.h"
#include "../Load/JSON/LoadJSON.h"
#include "../Load/LoadManager.h"
#include <unordered_map>
#include <string>

 // 後でGameEnumに移す
 /*
  *	@brief	銃の種類
  *	@author kuu
  */
enum class Enemy {
	Invalid = -1,

	TutorialEnemy,	// チュートリアルの通常エネミー
	Stage1Enemy,	// ステージ1の通常エネミー
	Stage2Enemy,	// ステージ2の通常エネミー
	Stage3Enemy,	// ステージ3の通常エネミー
	Stage4Enemy,	// ステージ4の通常エネミー
	Stage1Boss,		// ステージ1のボス
	Stage2Boss,		// ステージ2のボス
	Stage3Boss,		// ステージ3のボス
	Stage4Boss,		// ステージ4のボス


	Max
};

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
	std::unordered_map<Enemy, std::string> enemiesName;

private:
	EnemyDataManager() = default;
	~EnemyDataManager() = default;

private:
	/*
	 *	jsonデータをマップに登録
	 */
	void SetMapJsonData(const JSON& setData) {
		for (int i = 0; i < static_cast<int>(Enemy::Max); i++) {
			auto enemyName = enemiesName[static_cast<Enemy>(i)];

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
		enemiesName[Enemy::TutorialEnemy] = TUTORIAL_ENEMY;
		enemiesName[Enemy::Stage1Enemy] = STAGE1_ENEMY;
		enemiesName[Enemy::Stage2Enemy] = STAGE2_ENEMY;
		enemiesName[Enemy::Stage3Enemy] = STAGE3_ENEMY;
		enemiesName[Enemy::Stage4Enemy] = STAGE4_ENEMY;
		enemiesName[Enemy::Stage1Boss] = STAGE1_BOSS;
		enemiesName[Enemy::Stage2Boss] = STAGE2_BOSS;
		enemiesName[Enemy::Stage3Boss] = STAGE3_BOSS;
		enemiesName[Enemy::Stage4Boss] = STAGE4_BOSS;
		auto json = LoadManager::GetInstance().LoadResource<LoadJSON>(ENEMY_DATA_PATH);
		LoadManager::GetInstance().SetOnComplete([this, json]() {
			SetMapJsonData(json->GetData());
		});
	}

public:
	/*
	 *	各敵のデータ取得
	 */
	inline EnemyStatus GetWeaponData(Enemy enemy) {
		return enemyStatus[static_cast<int>(enemy)];
	}

};

#endif // !_ENEMYDATAMANAGER_H_