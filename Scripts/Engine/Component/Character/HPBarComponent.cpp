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
void ShowHPBar(VECTOR selfPos)
{
	// ワールド座標からスクリーン座標に変換
	Vector3 screenPos = ConvWorldPosToScreenPos(selfPos);

	int BoxX = screenPos.x;
	int BoxY = screenPos.y + 30;

	DrawBox(BoxX, BoxY, BoxX + 400, BoxY + 60, GetColor(50, 50, 50), true);
}
