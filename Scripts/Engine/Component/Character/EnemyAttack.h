/*
 *	@file	EnemyAttack.h
 *  @author kuu
 */
#ifndef _ENEMYATTACK_H_
#define _ENEMYATTACK_H_

#include "EnemyState.h"

/*
 *	“G‚ÌUŒ‚ˆ—Šî’êƒNƒ‰ƒX
 */
class EnemyAttackBase : public EnemyState {
private:
	//GameObjectPtr player = CameraManager::GetInstance().GetTarget();
public:
	EnemyAttackBase() = default;
	virtual ~EnemyAttackBase() = default;
public:
	virtual void Attack(GameObject* enemy, float deltaTime) = 0;
};

/*
 *	’Êí“G‚ÌUŒ‚ˆ—ƒNƒ‰ƒX
 */
class NormalEnemyAttack : public EnemyAttackBase {
	float coolTime = 3;
public:
	void Attack(GameObject* enemy, float deltaTime) override {
		coolTime -= deltaTime;
		//enemy = GetOwner();
		if (enemy->GetComponent<EnemyComponent>()->GetClosePlayer()) {
			if (coolTime > 0) return;
		// ‘O•û‚É“–‚½‚è”»’è‚ðo‚·
			enemy->GetComponent<ColliderBase>()->isCollider = true;
			coolTime = 3;
			// “–‚½‚Á‚½‚çƒ_ƒ[ƒW‚ð—^‚¦‚é
			//AddDamage(3);
		}
		else {
			enemy->GetComponent<ColliderBase>()->isCollider = false;
		}

	}
};





#endif // !_ENEMYATTACK_H_
