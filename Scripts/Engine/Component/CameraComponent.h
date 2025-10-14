/*
 *	@file	CameraComponent.h
 *  @autor	Riku
 */

#ifndef _CAMERACOMPONENT_H_
#define _CAMERACOMPONENT_H_

#include "Component.h"
#include "../Engine.h"

class CameraComponent : public Component {
public:
	CameraComponent() = default;
	virtual ~CameraComponent() = default;

public:
	virtual void Update(float deltaTime) override {

	}
	

};
// •Ê–¼’è‹`
using CameraComponentPtr = std::shared_ptr<CameraComponent>;

#endif // !_CAMERACOMPONENT_H_
