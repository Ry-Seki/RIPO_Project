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
#include "EnemyDeath.h"
#include "BulletComponent.h"
#include "HPBarComponent.h"
#include "../CameraComponent.h"
 /*
  *	コンストラクタ
  */
EnemyComponent::EnemyComponent()
	: EnemyComponent(new EnemyChase())
{
}

EnemyComponent::EnemyComponent(EnemyState* initState)
	: enemy(nullptr)
	, state(initState)
	, player(nullptr)
	, animator(nullptr)
	, wayPoint(0.0f, 0.0f, 0.0f)
	, nextWayPoint(0.0f, 0.0f, 0.0f)
	, wayPointDistance(1000.0f)
	, chaseTargetChangeFrag(false)
	, closePlayer(false)
	, attackIsTriger(false)
	, damageIsTriger(false)
	, turnDelay(0.0f)
	, modelHandle(0)
	, HP(0)
	, coolTime(2.0f)
	, TOP_VALUE(5000)
	, RANDOM_RANGE(100)
	, moveVec(Vector3::zero) {
}

EnemyComponent::~EnemyComponent()
{
	delete state;
}

void EnemyComponent::Start() {
	enemy = GetOwner();
	if (enemy == nullptr) return;

	if (state == nullptr)
		state = new EnemyChase();
	state->Start(enemy);

	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;

	animator = enemy->GetComponent<AnimatorComponent>();
	if (animator == nullptr) return;
	// 仮モデルハンドルの読み込み
	modelHandle = MV1LoadModel("Res/Model/Enemy/TutorialEnemy/EnemyModel.mv1");
	// モデルハンドルのセット
	animator->SetModelHandle(modelHandle);

	animator->LoadIndex(true);

	// 敵のデータ取得
	status = EnemyDataManager::GetInstance().GetEnemyData(GameEnum::EnemyType::Stage4Enemy);
	HP = status.HP;


}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void EnemyComponent::Update(float deltaTime) {
	// 移動量を初期化
	moveVec = Vector3::zero;

	if (state == nullptr || enemy == nullptr) return;
	state->Update(enemy, deltaTime);

	// ステージとの当たり判定
	StageManager::GetInstance().StageCollider(enemy, moveVec);

	coolTime -= deltaTime;

	VECTOR position = ToVECTOR(enemy->position);

	//auto camera = GetCameraViewMatrix();
	//VECTOR forward;
	//forward.x = -camera.m[2][0];
	//forward.y = -camera.m[2][1];
	//forward.z = -camera.m[2][2];
	//forward = VNorm(forward);
	//
	//GameObjectPtr cameraObj = CameraManager::GetInstance().GetCamera();
	//float pitch = cameraObj->rotation.x;
	//float yaw = cameraObj->rotation.y;
	//VECTOR OBcamera = ToVECTOR(DxForwardDir(cameraObj->rotation));
	//OBcamera = VNorm(OBcamera);
	//
	//float dot = VDot(OBcamera, forward);
	//dot = std::clamp(dot, -1.0f, 1.0f);
	//float angle = acosf(dot) * 180.0f / Pi;

	//MATRIX viewMat = /* あなたが直したView行列 */;
	//MATRIX projMat = /* 使用しているProjection行列 */;

	//SetCameraViewMatrix(viewMat);
	//SetCameraProjectionMatrix(projMat);

	// 線
	//VECTOR camPos = ToVECTOR(cameraObj->position);
	//DrawLine3D(
	//	camPos,
	//	VAdd(camPos, VScale(OBcamera, 50.0f)),
	//	GetColor(255, 0, 0) // 赤
	//);

	//VECTOR pos = GetCameraPosition();
	//VECTOR target = GetCameraTarget();

	//VECTOR DxForward = VNorm(VSub(target, pos));
	//DrawLine3D(
	//	pos,
	//	VAdd(pos, VScale(forward, 50.0f)),
	//	GetColor(0, 255, 0) // 緑
	//);

	//enemy->GetComponent<HPBarComponent>()->ShowHPBar(position);
}

/*
 *	衝突が起きたときに呼び出される処理
 */
void EnemyComponent::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other)
{
	if (coolTime <= 0) {
		attackIsTriger = false;
	}
	if (!attackIsTriger && other->GetOwner()->name == "Player") {
		// 当たったらダメージを与える
		auto playerStatus = player->GetComponent<PlayerComponent>()->GetPlayerStatus();
		// 今はとりあえず適当なダメージ
		playerStatus.HP = playerStatus.HP - status.attack;
		// ダメージを反映
		player->GetComponent<PlayerComponent>()->SetPlayerStatus(playerStatus);

		attackIsTriger = true;
	}

	// ダメージ判定
	if (!damageIsTriger && other->GetOwner()->name == "bullet") {
		// ダメージを受ける
		HP -= other->GetOwner()->GetComponent<BulletComponent>()->GetHitDamage();;
		if (HP <= 0) {
			HP = 0;
		}
		// 死亡判定
		if (HP <= 0 && state != nullptr) {
			damageIsTriger = true;
			state = new EnemyDeath();
			state->Start(enemy);
		}
		// 死ななかった場合
		else {
			firstAttackFlag = true;
		}
	}
	coolTime = 2;
}

Vector3 EnemyComponent::DxForwardDir(const Vector3& rotation)
{
	float pitch = rotation.x; // rad
	float yaw = rotation.y; // rad

	Vector3 dir;
	dir.x = -sinf(yaw) * cosf(pitch);
	dir.y = sinf(pitch);
	dir.z = -cosf(yaw) * cosf(pitch);
	return dir.Normalized();
}
