/*
 *	@file	PlayerUI.cpp
 *	@autor	Riku
 */

#include "PlayerUI.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../GameConst.h"

/*
 * 初期化
 */
void PlayerUI::Initialize(Engine& engine) {
	LoadManager& load = LoadManager::GetInstance();
	auto reticleSprite = load.LoadResource<LoadSprite>(RETICLE_PATH);
	load.SetOnComplete([this, reticleSprite]() {
		SetReticleGraphHandle(reticleSprite->GetHandle());
		});
}

/*
 *	@brief	メニューを開く
 */
void PlayerUI::Open() {
	isVisible = true;
	isInteractive = true;
	isStart = false;
}

/*
 *	描画
 */
void PlayerUI::Render() {
	// レティクル画像
	// 画像描画位置を計算
	float centerWidth = GameConst::WINDOW_WIDTH * 0.5f;
	float centerHeight = GameConst::WINDOW_HEIGHT * 0.5f;
	int graphSizeWidth;
	int graphSizeHeight;
	if (!reticleGraphHandle) return;
	GetGraphSize(reticleGraphHandle, &graphSizeWidth, &graphSizeHeight);
	float drawPosX = centerWidth - graphSizeWidth * 0.5f;
	float drawPosY = centerHeight - graphSizeHeight * 0.5f;
	// 描画
	DrawGraph(drawPosX, drawPosY, reticleGraphHandle, TRUE);

}

/*
 *	レティクルのグラフハンドルのセット
 */
void PlayerUI::SetReticleGraphHandle(int setHandle) {
	reticleGraphHandle = setHandle;
}
