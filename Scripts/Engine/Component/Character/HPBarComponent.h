/*
 *	@file	HPBarComponent.h
 *	@author	kuu
 */
#ifndef _HPBARCOMPONENT_H_
#define _HPBARCOMPONENT_H_

#include "../Component.h"
#include "../../Engine.h"

class HPBarComponent : public Component {
private:

public:
	/*
	 *	コンストラクタ
	 */
	HPBarComponent();
	~HPBarComponent() = default;

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

};



#endif // !_HPBARCOMPONENT_H_
