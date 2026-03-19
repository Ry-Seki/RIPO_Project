/*
 *	StrengthComponent.h
 *	@author Riku
 */

#ifndef _STRENGTHCOMPONENT_H_
#define _STRENGTHCOMPONENT_H_

#include "Component.h"

class StrengthComponent : public Component {
private:
	float strength;

public:
	/*
	 *	セットアップ
	 */
	void Setup(float setValue) {
		strength = setValue;
	}

public:
	/*
	 *	ストレングスの取得
	 */
	inline float GetStrength() const { return strength; }
};

#endif // !_STRENGTHCOMPONENT_H_
