/*
 *	MoveComponent.h
 *	@author Riku
 */

#ifndef _MOVECOMPONENT_H_
#define _MOVECOMPONENT_H_

#include "Component.h"
#include "../VecMath.h"

class MoveComponent : public Component {
private:
	Vector3 velocity;	// 速度ベクトル

public:
	MoveComponent();
	~MoveComponent() override = default;

public:
	/*
	 *	更新処理
	 */
	void Update(float deltaTime) override;

public:
	/*
	 *	速度ベクトルをセット
	 */
	void SetVelocity(Vector3 direction, float speed);

};

#endif // !_MOVECOMPONENT_H_
