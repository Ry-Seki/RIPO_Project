/*
 *	@file	BulletComponent.h
 *  @author	Riku
 */

#ifndef _BULLETCOMPONENT_H_
#define _BULLETCOMPONENT_H_

#include "../Component.h"

class BulletComponent : public Component {
private:
	GameObject* bullet;
	float destroyTimeCount;		// ’e‚ª”jŠü‚³‚ê‚éŽžŠÔ

	const float SHOT_SPEED;		// ’e‚Ì‘¬‚³

public:
	BulletComponent();
	~BulletComponent() = default;

public:
	void Start() override;
	void Update(float deltaTime) override;
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;
};

#endif // !_BULLETCOMPONENT_H_
