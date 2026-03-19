/*
 *	StrengthComponent.h
 *	@author Riku
 */

#ifndef _STRENGTHCOMPONENT_H_
#define _STRENGTHCOMPONENT_H_

#include "Component.h"

class StrengthComponent : public Component {
private:
	float strength = -1;

public:
	/*
	 *	セットアップ
	 */
	void Setup(float setStrength) {
		strength = setStrength;
	}

public:
	/*
	 *	ストレングスの取得
	 */
	inline float GetStrength() const { return strength; }
};

#endif // !_STRENGTHCOMPONENT_H_
