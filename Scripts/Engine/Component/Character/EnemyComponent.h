/*
 *	@file	EnemyComponent.h
 *  @author kuu
 */
#ifndef _ENEMYCOMPONENT_H_
#define _ENEMYCOMPONENT_H_

#include "CharacterBase.h"
#include "EnemyState.h"
#include "../../Engine.h"
#include "../AnimatorComponent.h"

class EnemyComponent : public CharacterBase {
private:
	GameObject* enemy;
	EnemyState* state;
	GameObjectPtr player;
	std::shared_ptr<AnimatorComponent> animator;
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
	// 攻撃衝突判定フラグ
	bool attackIsTriger;
	// ダメージ衝突判定フラグ
	bool damageIsTriger;
	// 方向転換時のディレイ
	float turnDelay;
	// スポーンID
	int enemySpawnID = 0;
	// 仮モデルハンドル
	int modelHandle;
	// 疑似クールタイム(苦肉の策)
	float coolTime;

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

	/*
	 *	衝突が起きたときに呼び出される処理
	 */
	virtual void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;

public:
	/*
	 *	状態の変更
	 *  param[in]	EnemyState*	setValue	次の状態
	 */
	inline void SetState(EnemyState* setValue) {
		if (state) {
			state->Execute(*this);
			//delete state;
		}
		state = setValue;
		state->Start(enemy);
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
	 *	ウェイポイントの取得
	 *  @author oorui
	 */
	inline Vector3 GetWayPoint()const { return wayPoint; }

	/*
	 *	反対のウェイポイントの取得
	 *  @auhtor oorui
	 */
	inline Vector3 GetNextWayPoint()const { return nextWayPoint; }

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
