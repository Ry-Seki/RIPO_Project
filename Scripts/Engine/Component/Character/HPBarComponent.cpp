#include "HPBarComponent.h"
#include "EnemyComponent.h"

/*
 *	コンストラクタ
 */
HPBarComponent::HPBarComponent()
	: player(nullptr)
	, camera(nullptr)
	, enemy(nullptr)
	, maxHP(0)
	, currentHP(0.0f)
	, displayHP(0.0f)
	, halfValue(0.5f)
{
}

/*
 *	更新処理の前に呼び出す処理
 */
void HPBarComponent::Start()
{
	player = CameraManager::GetInstance().GetTarget();
	if (player == nullptr) return;
	camera = CameraManager::GetInstance().GetCamera();
	if (camera == nullptr) return;
	enemy = GetOwner();
	if (enemy == nullptr) return;
	maxHP = enemy->GetComponent<EnemyComponent>()->GetEnemyMaxHP();
	currentHP = maxHP;

	displayHP = maxHP;
}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void HPBarComponent::Update(float deltaTime)
{
	currentHP = enemy->GetComponent<EnemyComponent>()->GetEnemyHP();

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
void HPBarComponent::ShowHPBar()
{
	// ダメージを受けるまで表示しない
	if (enemy->GetComponent<EnemyComponent>()->GetFirstAttackFlag()) {
		VECTOR enemyPos = ToVECTOR(enemy->position);

		// 敵の頭上
		VECTOR headPos = enemyPos;
		headPos.y += 700 + 10.0f;

		//VECTOR playerPos = ToVECTOR(player->position);
		VECTOR cameraPos = ToVECTOR(camera->position);

		// 視線ベクトル
		VECTOR viewDir = VSub(headPos, cameraPos);
		viewDir = VNorm(viewDir);

		// 上方向（ワールドY）
		VECTOR up = VGet(0.0f, 1.0f, 0.0f);

		// カメラの右方向ベクトル
		VECTOR rightDir = VCross(up, viewDir);
		rightDir = VNorm(rightDir);

		float worldHalfBar = 200.0f;

		// 左右端をカメラ右方向に作る
		VECTOR leftPos = VSub(headPos, VScale(rightDir, worldHalfBar));
		VECTOR rightPos = VAdd(headPos, VScale(rightDir, worldHalfBar));

		// ワールド座標からスクリーン座標に変換
		VECTOR screenLeft = ConvWorldPosToScreenPos(leftPos);
		VECTOR screenRight = ConvWorldPosToScreenPos(rightPos);
		VECTOR screenCenter = ConvWorldPosToScreenPos(headPos);

		// カメラ裏なら表示しない
		if (screenCenter.z > 0.999f || screenCenter.z < 0.9f) return;

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