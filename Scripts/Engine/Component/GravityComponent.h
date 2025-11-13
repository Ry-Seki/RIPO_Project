/*
 *	@file	GravityComponent.h
 *  @author Riku
 */

#ifndef _GRAVITYCOMPONENT_H_
#define _GRAVITYCOMPONENT_H_

#include "Component.h"
#include "../VecMath.h"

 /*
  *	重力
  */
class GravityComponent : public Component {
private:
	float fallSpeed;					// 落下速度

	const float FALL_SPEED_MAX;			// 最大落下速度
	const float GRAVITY_ACCELERATION;	// 重力加速度
public:
	GravityComponent();
	~GravityComponent() = default;

public:
	void Update(float deltaTime) override;

public:


};

#endif // !_GRAVITYCOMPONENT_H_
