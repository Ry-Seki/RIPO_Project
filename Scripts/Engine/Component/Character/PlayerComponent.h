/*
 *	@file	PlayerComponent.h
 *  @author	Riku
 */
#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#include "CharacterBase.h"
#include "../../Engine.h"

class PlayerComponent : public CharacterBase {
public:
	PlayerComponent() = default;
	virtual ~PlayerComponent() = default;

public:
	virtual void Update(float deltaTime) override;

};

#endif // !_PLAYERCOMPONENT_H_
