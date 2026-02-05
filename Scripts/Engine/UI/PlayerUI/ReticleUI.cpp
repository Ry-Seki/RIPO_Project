/*
 *	@file	ReticleUI.cpp
 *	@autor	Riku
 */

#include "ReticleUI.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../GameConst.h"

ReticleUI::ReticleUI()
	: reticleGraphHandle(-1) 
{}

/*
 * 初期化
 */
void ReticleUI::Initialize() {
	LoadManager& load = LoadManager::GetInstance();
	auto reticleSprite = load.LoadResource<LoadSprite>(RETICLE_PATH);
	load.SetOnComplete([this, reticleSprite]() {
		SetGraphHandle(reticleSprite->GetHandle());
		});
}

/*
 *	描画
 */
void ReticleUI::Render() {
	// 画像描画位置を計算
	float centerWidth = GameConst::WINDOW_WIDTH * 0.5f;
	float centerHeight = GameConst::WINDOW_HEIGHT * 0.5f;
	int graphSizeWidth;
	int graphSizeHeight;
	GetGraphSize(reticleGraphHandle, &graphSizeWidth, &graphSizeHeight);
	float drawPosX = centerWidth - graphSizeWidth * 0.5f;
	float drawPosY = centerHeight - graphSizeHeight * 0.5f;

	// 描画
	DrawGraph(drawPosX, drawPosY, reticleGraphHandle, TRUE);

}

/*
 *	グラフハンドルのセット
 */
void ReticleUI::SetGraphHandle(int setHandle) {
	reticleGraphHandle = setHandle;
}
