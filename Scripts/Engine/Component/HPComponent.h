/*
 *	HPComponent.h
 *	@author Riku
 */

#ifndef _HPCOMPONENT_H_
#define _HPCOMPONENT_H_

#include "Component.h"
#include "../Manager/ScreenEffectManager.h"
#include "../Manager/CameraManager.h"
#include "../GameConst.h"

class HPComponent : public Component {
private:
	float HP = 0.0f;		// 現在HP
	float maxHP = 0.0f;		// 最大HP
	bool isDead = false;	// 死んでるかどうか

public:
	/*
	 *	セットアップ
	 */
	void Setup(float setHP) {
		HP = setHP;
		maxHP = setHP;
	}

	/*
	 *	ダメージを与える
	 */
	void AddDamage(float damage) {
		if (HP - damage <= 0) {
			if (!isDead) {
				HP = 0;
				isDead = true;
				// プレイヤーなら画面効果再生
				if (GetOwner()->name == GameConst::_CREATE_POSNAME_PLAYER) {
					ScreenEffectManager::GetInstance().DamageFlash();
					CameraManager::GetInstance().CameraShake();
				}
			}
		}
		else {
			HP -= damage;
			// プレイヤーなら画面効果再生
			if (GetOwner()->name == GameConst::_CREATE_POSNAME_PLAYER) {
				ScreenEffectManager::GetInstance().DamageFlash();
				CameraManager::GetInstance().CameraShake();
			}
		}
	}

public:
	/*
	 *	HPの取得
	 */
	inline float GetHP() const { return HP; }

	/*
	 *	maxHPの取得
	 */
	inline float GetMaxHP() const { return maxHP; }

	/*
	 *	死んでるか否か
	 */
	inline bool IsDead() const { return isDead; }
};
// 別名定義
using HPComponentPtr = std::shared_ptr<HPComponent>;

#endif // !_HPCOMPONENT_H_
