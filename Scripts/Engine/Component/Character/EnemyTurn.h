/*
 *	@file	EnemyTurn.h
 *  @author kuu
 */
#ifndef _ENEMYTURN_H_
#define _ENEMYTURN_H_

#include "EnemyState.h"

/*
 *	敵の回転処理クラス
 */
class EnemyTurn : public EnemyState {
private:
	GameObject* enemy;
	CharacterBasePtr character;

private:
	// 回転スピード
	const float ROTATE_SPEED;
public:
	EnemyTurn();
	~EnemyTurn() = default;

public:
	/*
	 *	更新処理の前に呼び出す処理
	 */
	virtual void Start(EnemyComponent& enemy) override;

	/*
	 *	更新処理
	 *  param[in]	float	deltaTime
	 */
	virtual void Update(EnemyComponent& enemy, float deltaTime) override;
};

#endif // !_ENEMYTURN_H_
