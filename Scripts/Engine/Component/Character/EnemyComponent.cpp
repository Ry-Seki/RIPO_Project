/*
 *	@file	EnemyComponent.cpp
 *  @author kuu
 */
#include "EnemyComponent.h"
#include "../../Vision.h"
#include "../../Manager/CameraManager.h"
#include "../../Manager/StageManager.h"
#include "EnemyAttack.h"
#include "EnemyChase.h"
#include "EnemyTurn.h"
 /*
  *	コンストラクタ
  */
EnemyComponent::EnemyComponent(EnemyState* initState)
	: state(initState)
	, wayPoint(0.0f, 0.0f, 0.0f)
	, nextWayPoint(0.0f, 0.0f, 0.0f)
	, wayPointDistance(1000.0f)
	, enemy(nullptr)
	, chaseTargetChangeFrag(false)
	, closePlayer(false)
	, turnDelay(0)
	, TOP_VALUE(5000)
	, RANDOM_RANGE(100)
	, ROTATE_SPEED(3.0f)
	, MOVE_SPEED(700.0f)
	, DIFFERENCE_PLAYER(700) {
	state->Start(*this);
}

EnemyComponent::~EnemyComponent()
{
	delete state;
}

void EnemyComponent::Start() {
	enemy = GetOwner();
	if (enemy == nullptr) return;
}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void EnemyComponent::Update(float deltaTime) {
	// 移動量を初期化
	moveVec = Vector3::zero;

	CharacterBasePtr enemy = this;

	state->Update(*this, deltaTime);

	// ステージとの当たり判定
	StageManager::GetInstance().StageCollider(enemy, moveVec);
}