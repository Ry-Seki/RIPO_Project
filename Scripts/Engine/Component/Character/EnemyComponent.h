/*
 *	@file	EnemyComponent.h
 *  @author kuu
 */
#ifndef _ENEMYCOMPONENT_H_
#define _ENEMYCOMPONENT_H_

#include "CharacterBase.h"
#include "EnemyState.h"
#include "../../Engine.h"

class EnemyComponent : public CharacterBase {
private:
	GameObject* enemy;
	EnemyState* state;
	// ウェイポイント
	Vector3 wayPoint;
	// 目先のウェイポイント予定地
	Vector3 nextWayPoint;
	// ウェイポイント間の距離
	float wayPointDistance;
	// ウェイポイント追尾切り替えフラグ
	bool chaseTargetChangeFrag;
	// プレイヤーの距離判定フラグ
	bool closePlayer;
	// 方向転換時のディレイ
	float turnDelay;
	// スポーンID
	int enemySpawnID = 0;

	// ランダムで加算されるカウントの天井値
	const float TOP_VALUE;
	// ランダムで加算される値の範囲
	const int RANDOM_RANGE;
	// 移動量
	Vector3 moveVec;

public:
	/*
	 *	コンストラクタ
	 */
	EnemyComponent();
	EnemyComponent(EnemyState* initState);
	~EnemyComponent();

public:
	/*
	 *	更新処理の前に呼び出す処理
	 */
	virtual void Start() override;

	/*
	 *	更新処理
	 *  param[in]	float	deltaTime
	 */
	virtual void Update(float deltaTime) override;

private:

public:
	/*
	 *	状態の変更
	 *  param[in]	EnemyState*	setValue	次の状態
	 */
	inline void SetState(EnemyState* setValue) {
		state->Execute(*this);
		state = setValue;
		state->Start(*this);
	}

	/*
	 *	ウェイポイントの位置変更
	 *  param[in]	Vector3&	setValue	敵の座標
	 */
	inline void SetWayPoint(Vector3& setValue) {
		wayPoint = Vector3(setValue.x, setValue.y, setValue.z + wayPointDistance);
		nextWayPoint = Vector3(setValue.x, setValue.y, setValue.z - wayPointDistance);
	}

	/*
	 *	プレイヤーの距離判定の取得
	 */
	inline bool GetClosePlayer() const { return closePlayer; }

	/*
     *	ウェイポイント追尾切り替えフラグの取得
	 */
	inline bool GetChaseTargetChangeFrag() const { return chaseTargetChangeFrag; }

	/*
	 *	ウェイポイント追尾切り替えフラグの変更
	 */
	inline void SetChaseTargetChangeFrag(bool setValue) {
		chaseTargetChangeFrag = setValue;
	}

	/*
	 *	位置の取得
	 */
	inline Vector3 GetEnemyPosition() const {
		if (enemy == nullptr) return Vector3::zero;
		return enemy->position;
	}

	/*
	 *	位置の変更
	 */
	inline void SetEnemyPosition(Vector3 setValue) {
		enemy->position = setValue;
	}

	/*
	 *	角度の取得
	 */
	inline Vector3 GetEnemyRotation() const { return enemy->rotation; }
	
	/*
	 *	角度の変更
	 */
	inline void SetEnemyRotation(Vector3 setValue) {
		enemy->rotation = setValue;
	}



	/*
	 *	スポーンIDの取得
	 */
	inline int GetSpawnEnemyID() const { return enemySpawnID; }

	/*
	 *	スポーンIDの変更
	 */
	inline void SetSpawnEnemyID(int& setValue) {
		enemySpawnID = setValue;
	}
};

#endif // !_ENEMYCOMPONENT_H_
