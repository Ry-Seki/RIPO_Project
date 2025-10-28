/*
 *	@file	EnemyComponent.h
 *  @author kuu
 */
#ifndef _ENEMYCOMPONENT_H_
#define _ENEMYCOMPONENT_H_

#include "CharacterBase.h"
#include "../../Engine.h"

class EnemyComponent : public CharacterBase {
private:
	// 移動スピード
	float moveSpeed;
	// ウェイポイント
	Vector3 wayPoint;
	// 目先のウェイポイント予定地
	Vector3 nextWayPoint;

public:
	/*
	 *	コンストラクタ
	 */
	EnemyComponent();
	virtual ~EnemyComponent() = default;

public:
	/*
	 *	更新処理の前に呼び出す処理
	 */
	virtual void Start() override;

	/*
	 *	更新処理
	 */
	virtual void Update(float deltaTime) override;

private:
	/*
	 *	移動処理
	 */
	void EnemyMove(GameObject* enemy, float deltaTime);
};

#endif // !_ENEMYCOMPONENT_H_
