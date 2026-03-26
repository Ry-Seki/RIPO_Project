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
	Vector3 velocity;			// 速度ベクトル
	Vector3 moveVec;			// 移動量
	bool isMove;				// 移動中フラグ

	const float INERTIA_RATE;	// 慣性倍率

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
	 *	@param	direction	方向
	 *  @param	speed		速度
	 */
	void SetVelocity(Vector3 direction, float speed);

	/*
	 *	移動量取得
	 *	@return Vector3
	 */
	inline Vector3 GetMoveVec() const { return moveVec; }

	/*
	 *	移動中フラグ取得
	 *  @return bool
	 */
	inline bool IsMove() const { return isMove; }

};
// 別名定義
using MoveComponentPtr = std::shared_ptr<MoveComponent>;

#endif // !_MOVECOMPONENT_H_
