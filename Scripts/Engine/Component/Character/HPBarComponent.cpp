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
void HPBarComponent::ShowHPBar(VECTOR selfPos)
{
	// ワールド座標からスクリーン座標に変換
	Vector3 screenPos = ConvWorldPosToScreenPos(selfPos);
	DrawFormatString(0, 130, GetColor(255, 255, 255), "screenPos(%f,%f,%f)",
		screenPos.x, screenPos.y, screenPos.z);

	int BoxX = screenPos.x;
	int BoxY = screenPos.y + 100;

	/*if (BoxX > 0 && BoxY > 0) {

	}*/

	DrawBox(BoxX, BoxY, BoxX + 40, BoxY + 6, GetColor(50, 50, 50), true);
}