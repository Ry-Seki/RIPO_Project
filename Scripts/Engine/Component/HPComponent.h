/*
 *	HPComponent.h
 *	@author Riku
 */

#ifndef _HPCOMPONENT_H_
#define _HPCOMPONENT_H_

#include "Component.h"

class HPComponent : public Component {
private:
	float HP = 0.0f;		// 現在HP
	float maxHP = 0.0f;		// 最大HP
	bool isDead = false;		// 死んでるかどうか

public:
	/*
	 *	セットアップ
	 */
	void Setup(float setHP) {
		HP = setHP;
		maxHP = setHP;
	}

	/*
	 *	ダメージを与える
	 */
	void AddDamage(float damage) {
		if (HP - damage <= 0) {
			HP = 0;
			isDead = true;
		}
		else {
			HP - damage;
		}
	}

public:
	/*
	 *	HPの取得
	 */
	inline float GetHP() const { return HP; }

	/*
	 *	maxHPの取得
	 */
	inline float GetMaxHP() const { return maxHP; }

	/*
	 *	死んでるか否か
	 */
	inline bool IsDead() const { return isDead; }
};

#endif // !_HPCOMPONENT_H_
