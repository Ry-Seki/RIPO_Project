/*
 *	@file	EnemyTurn.cpp
 *	@author kuu
 */
#include "EnemyTurn.h"
#include "EnemyChase.h"
#include "../../Manager/CharacterManager.h"
#include "CharacterUtility.h"
#include "EnemyAttack.h"
#include "../../Vision.h"
#include "../../Manager/CameraManager.h"

using namespace CharacterUtility;

EnemyTurn::EnemyTurn()
	: enemyComponent(nullptr)
	, player(nullptr)
	, ROTATE_SPEED(3.0f)
	, targetAngle(0.0f) {
}

/*
 *	XVˆ—‚Ì‘O‚ÉŒÄ‚Ño‚·ˆ—
 *  param[in]	EnemyComponent&	enemy
 */
void EnemyTurn::Start(GameObject* enemy) {
	enemyComponent = enemy->GetComponent<EnemyComponent>();

	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;

	// ‰ñ“]‚Ì–Ú•WˆÊ’u
	Vector3 targetPos;
	// ˆÚ“®ˆÊ’u‚ª+‚Ìê‡
	if (!enemyComponent->GetChaseTargetChangeFrag()) {
		// ’Êí‚ÌWayPoint‚ðŒü‚­
		targetPos = enemyComponent->GetWayPoint();
	}
	// ˆÚ“®ˆÊ’u‚ª-‚Ìê‡
	else {
		// ”½‘Î‘¤‚ÌWayPoint‚ðŒü‚­
		targetPos = enemyComponent->GetNextWayPoint();
	}

	// Ž©g‚©‚ç–Ú•W‚Ö‚Ì•ûŒüƒxƒNƒgƒ‹‚ðŒvŽZ
	Vector3 direction = Direction(enemyComponent->GetEnemyPosition(), targetPos);

	// YŽ²‰ñ“]‚Ì–Ú•WŠp“x‚ð‚¾‚·
	targetAngle = atan2(direction.x, direction.z);
	// ”½‘Îƒ‚ƒfƒ‹—p‚É•â³
	targetAngle += Pi;

}

/*
 *	XVˆ—
 *  param[in]	GameObject*	enemy
 *  param[in]	float	deltaTime
 */
void EnemyTurn::Update(GameObject* enemy, float deltaTime) {
	if (enemy == nullptr)return;

	// “G‚ÌƒRƒ“ƒ|[ƒlƒ“ƒg‚ðŽæ“¾
	auto enemyComponent = enemy->GetComponent<EnemyComponent>();

	// Šp“x‚ð³‹K‰»
	while (targetAngle > Pi)  targetAngle -= Pi * 2;
	while (targetAngle < -Pi) targetAngle += Pi * 2;

	// Œ»ÝŠp“x‚Æ‚Ì·‚ð‹‚ß‚é
	float angleDiff = targetAngle - enemy->rotation.y;

	// Šp“x·‚ð³‹K‰»
	while (angleDiff > Pi)  angleDiff -= Pi * 2;
	while (angleDiff < -Pi) angleDiff += Pi * 2;

	// ‰ñ“]‚ª‘«‚è‚È‚¯‚ê‚Î
	if (fabs(angleDiff) > Deg2Rad * 1.0f) {
		// ‰ñ“]•ûŒü‚ðŒˆ’è‚µ‚Ä™X‚É‰ñ‚·
		float rotateDir = (angleDiff > 0) ? 1.0f : -1.0f;
		enemy->rotation.y += rotateDir * ROTATE_SPEED * deltaTime;
	}
	else {
		// Šp“x‚ð‡‚í‚¹‚é
		enemy->rotation.y = targetAngle;

		// ˆÚ“®ó‘Ô‚Ö‘JˆÚ
		enemyComponent->SetState(new EnemyChase());
	}

	// Ž‹ŠE”»’è
	if (player && Vision(enemy->position, -ForwardDir(enemy->rotation), player->position, 30, 2000)) {
		enemyComponent->SetState(new EnemyChase());
	}
}