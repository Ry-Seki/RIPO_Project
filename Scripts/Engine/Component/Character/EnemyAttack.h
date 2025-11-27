/*
 *	@file	EnemyAttack.h
 *  @author kuu
 */
#ifndef _ENEMYATTACK_H_
#define _ENEMYATTACK_H_

#include "EnemyComponent.h"

/*
 *	“G‚ÌUŒ‚ˆ—Šî’êƒNƒ‰ƒX
 */
class EnemyAttackBase : public Component {
private:
	GameObjectPtr player = CameraManager::GetInstance().GetTarget();;
	if (player == nullptr) return;
public:
	EnemyAttackBase() = default;
	virtual ~EnemyAttackBase() = default;
public:
	virtual void Attack() = 0;

	///*
	// *	ƒ_ƒ[ƒW‚ð—^‚¦‚éˆ—
	// */
	//void AddDamage(int enemyPower) {
	//	int playerHP = player->AddComponent<CharacterBase>()->GetHP();
	//	playerHP -= enemyPower;
	//	player->AddComponent<CharacterBase>()->SetHP(playerHP);
	//}
};

/*
 *	’Êí“G‚ÌUŒ‚ˆ—ƒNƒ‰ƒX
 */
class NormalEnemyAttack : public EnemyAttackBase {
	GameObject* enemy = GetOwner;
	if (enemy == nullptr) return;
	float coolTime = 3;
public:
	void Attack(float deltaTime) override {
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
