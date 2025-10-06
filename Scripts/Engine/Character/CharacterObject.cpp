/*
 *	@file	CharacterObject.cpp
 */

#include "CharacterObject.h"

#include <DxLib.h>

void CharacterObject::Start() {
	GameObject::Start();
}

void CharacterObject::Update(float deltaTime) {
	GameObject::Update(deltaTime);
}

void CharacterObject::Render() {
	if (modelID != -1) MV1DrawModel(modelID);
}

void CharacterObject::OnCollision(const ComponentPtr& self, const ComponentPtr& other) {
	GameObject::OnCollision(self, other);
}

void CharacterObject::OnDestroy() {
	GameObject::OnDestroy();
}
