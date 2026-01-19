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
	GameObjectPtr player;
	//GameObject* modelRenderer;
	std::shared_ptr<AnimatorComponent> animator;
	BossState* state;
	int animationHandle;
	int modelHandle;
	float coolTime;

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

	/*
	 *	衝突が起きたときに呼び出される処理
	 */
	virtual void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;

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

	/*
	 *	状態の変更
	 *  param[in]	BossState*	setValue	次の状態
	 */
	inline void SetState(BossState* setValue) {
		if (state) {
			state->Execute(*this);
		}
		state = setValue;
		state->Start(boss);
	}
};

#endif // !_BOSSCOMPONENT_H_