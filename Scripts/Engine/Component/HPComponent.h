/*
 *	HPComponent.h
 *	@author Riku
 */

#ifndef _HPCOMPONENT_H_
#define _HPCOMPONENT_H_

#include "Component.h"

class HPComponent : public Component {
private:
	float HP = -1;		// 現在HP
	float maxHP = -1;	// 最大HP
	bool dead = false;	// 死んでるかどうか

public:
	/*
	 *	セットアップ
	 */
	void Setup(float setValue) {
		HP = setValue;
		maxHP = setValue;
	}

	/*
	 *	ダメージを与える
	 */
	void AddDamage(float damage) {
		if (HP - damage <= 0) {
			HP = 0;
			dead = true;
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
	inline bool IsDead() const { return dead; }
};

#endif // !_HPCOMPONENT_H_
