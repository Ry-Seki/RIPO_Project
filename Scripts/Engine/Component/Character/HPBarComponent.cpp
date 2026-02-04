#include "HPBarComponent.h"

/*
 *	コンストラクタ
 */
HPBarComponent::HPBarComponent()
{
}

/*
 *	更新処理の前に呼び出す処理
 */
void HPBarComponent::Start()
{
}

/*
 *	更新処理
 *  param[in]	float	deltaTime
 */
void HPBarComponent::Update(float deltaTime)
{
	
}

/*
 *	HPバーを表示させる
 *	param[in]	VECTOR	selfPos		自分自身のポジション
 */
void HPBarComponent::ShowHPBar()
{
	auto enemy = GetOwner();
	VECTOR enemyPos = ToVECTOR(enemy->position);

	VECTOR headPos = enemyPos;
	headPos.y += 700 + 10.0f;

	// ワールド座標からスクリーン座標に変換
	Vector3 screenPos = DxLib::ConvWorldPosToScreenPos(headPos);
	DrawFormatString(0, 130, GetColor(255, 255, 255), "screenPos(%f,%f,%f)",
		screenPos.x, screenPos.y, screenPos.z);

	// カメラ裏なら表示しない
	if (screenPos.z > 1.0f) return;

	float scale = 1.0f / screenPos.z;
	scale = std::clamp(scale, 0.4f, 1.5f);

	int BoxX = static_cast<int>(screenPos.x);
	int BoxY = static_cast<int>(screenPos.y);

	float halfWidth = 40.0f * scale;
	float halfHeight = 6.0f * scale;
	DrawBox(BoxX - halfWidth, BoxY - halfHeight, BoxX + halfWidth, BoxY + halfHeight, GetColor(0, 250, 0), true);
}