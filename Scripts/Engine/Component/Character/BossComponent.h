/*
 *	@file	BossComponent.h
 *  @author kuu
 */
#ifndef _BOSSCOMPONENT_H_
#define _BOSSCOMPONENT_H_

#include "CharacterBase.h"
#include "BossState.h"
#include "../AnimatorComponent.h"
#include "../ModelRenderer.h"

class BossComponent : public CharacterBase {
private:
	GameObject* boss;
	//GameObject* modelRenderer;
	std::shared_ptr<AnimatorComponent> animator;
	BossState* state;
	int animationHandle;
	int modelHandle;

public:
	/*
	 *	コンストラクタ
	 */
	BossComponent();
	BossComponent(BossState* initState);
	~BossComponent();

public:
	virtual void Start() override;
	/*
	 *	更新処理
	 */
	virtual void Update(float deltaTime) override;

public:
	/*
	 *	位置の取得
	 */
	inline Vector3 GetBossPosition() const {
		if (boss == nullptr) return Vector3::zero;
		return boss->position;
	}

	/*
	 *	位置の変更
	 */
	inline void SetBossPosition(Vector3 setValue) {
		boss->position = setValue;
	}

	/*
	 *	角度の取得
	 */
	inline Vector3 GetBossRotation() const { return boss->rotation; }

	/*
	 *	角度の変更
	 */
	inline void SetBossRotation(Vector3 setValue) {
		boss->rotation = setValue;
	}

};

#endif // !_BOSSCOMPONENT_H_