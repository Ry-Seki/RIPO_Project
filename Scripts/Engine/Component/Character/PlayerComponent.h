/*
 *	@file	PlayerComponent.h
 *  @author	Riku
 */
#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#include "CharacterBase.h"
#include "../../Engine.h"

class PlayerComponent : public CharacterBase {
private:
	float moveSpeed;		// ˆÚ“®‘¬“x
	float acceleration;		// ‰Á‘¬“x

	const float ACCELERATION_MAX;
	const float ACCELERATION_RATE;
	const float DEFAULT_MOVE_SPEED;

public:
	PlayerComponent();
	virtual ~PlayerComponent() = default;

public:
	virtual void Update(float deltaTime) override;

private:
	void PlayerMove();
	void PlayerRun(float deltaTime);

};

#endif // !_PLAYERCOMPONENT_H_
