/*
 *	@file	BulletComponent.h
 *  @author	Riku
 */

#ifndef _BULLETCOMPONENT_H_
#define _BULLETCOMPONENT_H_

#include "../Component.h"
#include "../../VecMath.h"
#include "../../GameObject.h"

class BulletComponent : public Component {
private:
	GameObject* bullet;
	float destroyTimeCount;		// 弾が破棄される時間
	Vector3 moveDirection;		// 移動方向
	GameObjectPtr shotOwner;	// 射撃者
	float hitDamage;			// 当たった時に与えるダメージ
	float moveSpeed;			// 弾の速さ

public:
	BulletComponent();
	~BulletComponent() = default;

public:
	/*
	 *	更新処理
	 */
	void Update(float deltaTime) override;
	/*
	 *	衝突が起きたときに呼び出される処理
	 */
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;

public:
	/*
	 *	セットアップ
	 *	@param	setDirection	移動方向
	 *	@param	setScale		スケール
	 *	@param	setOwner		射撃者
	 *	@param	setDamage		当たった時に与えるダメージ
	 *	@param	setSpeed		弾の速さ
	 */
	void Setup(
		Vector3 setDirection,
		Vector3 setScale,
		GameObjectPtr setOwner,
		float setDamage,
		float setSpeed);

public:
	inline float GetHitDamage() { return hitDamage; }
};
using BulletComponentPtr = std::shared_ptr<BulletComponent>;

#endif // !_BULLETCOMPONENT_H_
