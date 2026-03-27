/*
 *	@file	BossHPBarComponent.cpp
 *	@author	kuu
 */
#include "BossHPBarComponentr.h"
#include "BossComponent.h"
#include "../../Scene/Scene.h"

/*
 *	コンストラクタ
 */
BossHPBarComponent::BossHPBarComponent()
	: player(nullptr)
	, boss(nullptr)
	, maxHP(0)
	, currentHP(0.0f)
	, displayHP(0.0f)
	, minScreenCenterZ(0.95f)
	, halfValue(0.5f)
	, DISTANCE_PLAYER(2000)
{
}

/*
 *	更新処理の前に呼び出す処理
 */
void BossHPBarComponent::Start()
{
	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;
	boss = GetOwner();
	if (boss == nullptr) return;
	//maxHP = boss->GetComponent<BossComponent>()->GetBossMaxHP();
	currentHP = maxHP;

	//displayHP = maxHP;
}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void BossHPBarComponent::Update(float deltaTime)
{
	currentHP = boss->GetComponent<HPComponent>()->GetHP();

	// ダメージバーの速度
	float speed = 100.0f;

	// ダメージバーを減らす処理
	if (displayHP > currentHP)
	{
		displayHP -= speed * deltaTime;
		if (displayHP < currentHP)
			displayHP = currentHP;
	}
}

/*
 *	HPバーを表示させる
 *	param[in]	VECTOR	selfPos		自分自身のポジション
 */
void BossHPBarComponent::ShowHPBar()
{
	// 壁判定RayCast
	Scene::RayCastHit hitInfo;
	auto camera = CameraManager::GetInstance().GetCamera();
	auto engine = camera->GetEngine();
	if (!boss)
		return;
	Ray bossRay = { camera->position, Direction(camera->position, boss->position) };
	if (camera == nullptr) return;
	auto boss = GetOwner();
	if (boss == nullptr) return;
	bool bossHit = engine->GetCurrentScene()->RayCast(
		bossRay, hitInfo,
		[this, camera, boss](const ColliderBasePtr& col, float distance) {
			// プレイヤーと自分以外のオブジェクト
			return !col &&
				distance < Distance(camera->position, boss->position);
		}
	);
	if (!bossHit) {
		VECTOR bossPos = ToVECTOR(boss->position);

		// 敵の頭上
		VECTOR headPos = bossPos;
		headPos.y += 800 + 10.0f;

		VECTOR cameraPos = ToVECTOR(camera->position);

		// 視線ベクトル
		VECTOR viewDir = VSub(headPos, cameraPos);
		viewDir = VNorm(viewDir);

		// 上方向（ワールドY）
		VECTOR up = VGet(0.0f, 1.0f, 0.0f);

		// カメラの右方向ベクトル
		VECTOR rightDir = VCross(up, viewDir);
		rightDir = VNorm(rightDir);

		float worldHalfBar = 700.0f;

		// 左右端をカメラ右方向に作る
		VECTOR leftPos = VSub(headPos, VScale(rightDir, worldHalfBar));
		VECTOR rightPos = VAdd(headPos, VScale(rightDir, worldHalfBar));

		// ワールド座標からスクリーン座標に変換
		VECTOR screenLeft = ConvWorldPosToScreenPos(leftPos);
		VECTOR screenRight = ConvWorldPosToScreenPos(rightPos);
		VECTOR screenCenter = ConvWorldPosToScreenPos(headPos);

		auto player = CameraManager::GetInstance().GetTarget();
		// 距離が近い場合は範囲を広げる
		if (player && Distance(player->position, boss->position) < DISTANCE_PLAYER) {
			minScreenCenterZ = 0.9f;
		}
		else {
			minScreenCenterZ = 0.95f;
		}
		// カメラ裏なら表示しない
		if (screenCenter.z > 0.999f || screenCenter.z < minScreenCenterZ) return;

		// 画面上の長さ（距離で計算）
		float dx = screenRight.x - screenLeft.x;
		float dy = screenRight.y - screenLeft.y;
		float barWidth = sqrtf(dx * dx + dy * dy);
		float barHeight = sqrtf(dx * dx + dy * dy) * 0.1f;

		float greenRate = currentHP / maxHP;
		float redRate = displayHP / maxHP;

		int BoxX = screenCenter.x;
		int BoxY = screenCenter.y;

		// ダメージバー(赤バー)
		float redWidth = barWidth * redRate;
		DrawBox(
			BoxX - barWidth * halfValue,
			BoxY - barHeight * halfValue,
			BoxX - barWidth * halfValue + redWidth,
			BoxY + barHeight * halfValue,
			GetColor(255, 0, 0),
			true
		);
		// HPバー(緑バー)
		float greenWidth = barWidth * greenRate;
		DrawBox(
			BoxX - barWidth * halfValue,
			BoxY - barHeight * halfValue,
			BoxX - barWidth * halfValue + greenWidth,
			BoxY + barHeight * halfValue,
			GetColor(0, 250, 0),
			true
		);
		// 枠
		DrawBox(
			BoxX - barWidth * halfValue,
			BoxY - barHeight * halfValue,
			BoxX + barWidth * halfValue,
			BoxY + barHeight * halfValue,
			GetColor(0, 0, 0),
			false
		);
	}
}