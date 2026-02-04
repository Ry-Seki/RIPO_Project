/*
 *	@file	BulletComponent.h
 *  @author	Riku
 */

#ifndef _BULLETCOMPONENT_H_
#define _BULLETCOMPONENT_H_

#include "../Component.h"
#include "../../VecMath.h"

class BulletComponent : public Component {
private:
	GameObject* bullet;
	float destroyTimeCount;			// ’e‚ª”jŠü‚³‚ê‚éŠÔ
	Vector3 hitDirection;			// ’…’e’n“_•ûŒü
	float hitDamage;				// “–‚½‚Á‚½‚É—^‚¦‚éƒ_ƒ[ƒW

	const float SHOT_SPEED;			// ’e‚Ì‘¬‚³

public:
	BulletComponent();
	~BulletComponent() = default;

public:
	void Start() override;
	void Update(float deltaTime) override;
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;

public:
	inline float GetHitDamage() { return hitDamage; }
};

#endif // !_BULLETCOMPONENT_H_
