/*
 *	StaminaComponent.h
 *	@author Riku
 */

#ifndef _STAMINACOMPONENT_H_
#define _STAMINACOMPONENT_H_

#include "Component.h"

class StaminaComponent : public Component {
private:
	float stamina = 0.0f;								// 現在スタミナ
	float maxStamina = 0.0f;							// 最大スタミナ
	float staminaHealCoolTime = 0.0f;					// スタミナが回復し始めるまでの時間
	const float STAMINA_HEAL_COOL_TIME_MAX = 50.0f;		// スタミナ回復クールタイムの最大
	const float STAMINA_AUTO_HEAL_VALUE = 0.2f;			// スタミナの自動回復量

public:
	/*
	 *	更新処理
	 */
	void Update(float deltaTime) override {
		// クールタイムが開け次第スタミナ回復
		if (staminaHealCoolTime <= 0) {
			HealStamina(STAMINA_AUTO_HEAL_VALUE);
		}
		else {
			staminaHealCoolTime -= 1;
		}
	}

public:
	/*
	 *	セットアップ
	 */
	void Setup(float setStamina) {
		stamina = setStamina;
		maxStamina = setStamina;
	}

	/*
	 *	スタミナを使う
	 */
	void UseStamina(float useValue) {
		if (stamina - useValue <= 0) {
			stamina = 0;
		}
		else {
			stamina -= useValue;
		}
		// スタミナ自動回復クールタイム
		staminaHealCoolTime = STAMINA_HEAL_COOL_TIME_MAX;
	}

	/*
	 *	スタミナ回復
	 */
	void HealStamina(float healValue) {
		if (stamina + healValue >= maxStamina) {
			stamina = maxStamina;
		}
		else {
			stamina += healValue;
		}
	}

public:
	/*
	 *	スタミナの取得
	 */
	inline float GetStamina() const { return stamina; }

	/*
	 *	最大スタミナの取得
	 */
	inline float GetMaxStamina() const { return maxStamina; }
};
// 別名定義
using StaminaComponentPtr = std::shared_ptr<StaminaComponent>;

#endif // !_STAMINACOMPONENT_H_
