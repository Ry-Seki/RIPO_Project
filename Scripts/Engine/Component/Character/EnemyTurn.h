/*
 *	@file	EnemyTurn.h
 *  @author kuu
 */
#ifndef _ENEMYTURN_H_
#define _ENEMYTURN_H_

#include "EnemyComponent.h"

/*
 *	敵の回転処理クラス
 */
class EnemyTurn : public EnemyComponent {
private:

public:
	EnemyTurn();
	~EnemyTurn() = default;

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
};

#endif // !_ENEMYTURN_H_
