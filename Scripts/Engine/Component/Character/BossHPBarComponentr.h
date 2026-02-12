/*
 *	@file	BossHPBarComponent
 *  @author	kuu
 */
#ifndef _BOSSHPBARCOMPONENT_H_
#define _BOSSHPBARCOMPONENT_H_

#include "../Component.h"
#include "../../Engine.h"
#include "../../Manager/CameraManager.h"

class BossHPBarComponent : public Component {
private:
	GameObjectPtr player;
	GameObjectPtr camera;
	GameObject* boss;
	// 最大HP
	int maxHP;
	// 実際のHP
	float currentHP;
	// 遅れてくるHP
	float displayHP;

	const float halfValue;

public:
	/*
	 *	コンストラクタ
	 */
	BossHPBarComponent();
	~BossHPBarComponent() = default;

public:
	/*
	 *	更新処理の前に呼び出す処理
	 */
	virtual void Start() override;

	/*
	 *	更新処理
	 *  param[in]	float	deltaTime
	 */
	virtual void Update(float deltaTime) override;

	/*
	 *	HPバーを表示させる
	 *	param[in]	Vector3	selfPos		自分自身のポジション
	 */
	void ShowHPBar();

public:
	inline void SetMaxHP(int setValue) { maxHP = setValue; }
	inline void SetDisplayHP() { displayHP = maxHP; }

};

#endif // !_BOSSHPBARCOMPONENT_H_
