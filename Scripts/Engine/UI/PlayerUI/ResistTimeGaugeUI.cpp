/*
 *	@file	ResistTimeGaugeUI.cpp
 *	@author	Riku
 */

#include "ResistTimeGaugeUI.h"
#include "../../Component/Character/CharacterUtility.h"
#include "../../System/Status/PlayerStatusManager.h"
#include "../../GameConst.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"

using namespace CharacterUtility;

ResistTimeGaugeUI::ResistTimeGaugeUI() 
	: GAUGE_START_WIDTH_POS_RATIO(0.2f)
	, GAUGE_START_HEIGHT_POS_RATIO(0.93f)
	, GAUGE_END_POS_X_MOVE_RATE(0.6f)
	, GAUGE_END_HEIGHT_POS_RATIO(0.95f)
	, GRAPH_WIDTH_POS_RATIO(0.1f)
	, GRAPH_HEIGHT_POS_RATIO(0.9f)
{}

/*
 *	初期化処理
 */
void ResistTimeGaugeUI::Initialize() {
	LoadManager& load = LoadManager::GetInstance();
	auto resistSprite = load.LoadResource<LoadSprite>(RESIST_PATH);
	load.SetOnComplete([this, resistSprite]() {
		SetGraphHandle(resistSprite->GetHandle());
		});
}

/*
 *	描画処理
 */
void ResistTimeGaugeUI::Render() {
	// ゲージ描画位置計算
	float playerResist = GetPlayer()->GetComponent<ResistTimeComponent>()->GetResistTime();
	float baseResist = static_cast<float>(PlayerStatusManager::GetInstance().GetPlayerStatusData().base.resistTime);
	float posSX = GameConst::WINDOW_WIDTH * GAUGE_START_WIDTH_POS_RATIO;
	float posSY = GameConst::WINDOW_HEIGHT * GAUGE_START_HEIGHT_POS_RATIO;
	// ゲージの横幅はプレイヤーのレジスト状況に応じて減っていく
	float changeValue = GAUGE_START_WIDTH_POS_RATIO;
	if (playerResist > 0)
		changeValue = GAUGE_START_WIDTH_POS_RATIO + (playerResist / baseResist * GAUGE_END_POS_X_MOVE_RATE);
	float posEX = GameConst::WINDOW_WIDTH * changeValue;
	float posEY = GameConst::WINDOW_HEIGHT * GAUGE_END_HEIGHT_POS_RATIO;
	// レジストが半分になると色が変っていき、最終的に赤になる
	auto yg = GameConst::COLOR_YELLOW_GREEN;
	auto r = GameConst::COLOR_RED;
	if (playerResist <= baseResist * 0.5f) {
		float changeColorRate = 1 - playerResist / (baseResist * 0.5f);
		yg.x += changeColorRate * (r.x - yg.x);
		yg.y += changeColorRate * (r.y - yg.y);
		yg.z += changeColorRate * (r.z - yg.z);
	}

	// ゲージ描画
	DrawBox(posSX, posSY, posEX, posEY, GetColor(yg.x, yg.y, yg.z), TRUE);
	
	// ゲージ枠描画
	auto basePosEX = GameConst::WINDOW_WIDTH * (GAUGE_START_WIDTH_POS_RATIO + GAUGE_END_POS_X_MOVE_RATE);
	auto w = GameConst::COLOR_WHITE;
	DrawBox(posSX, posSY, basePosEX, posEY, GetColor(w.x, w.y, w.z), FALSE);

	// ゲージメモリ描画
	// ゲージを4等分するメモリ
	float gaugeWidth = basePosEX - posSX;
	float gaugeHeight = posEY - posSY;
	float lineX = gaugeWidth * 0.25f + posSX;
	float lineY = gaugeHeight * 0.75f + posSY;
	DrawLine(lineX, lineY, lineX, posEY, GetColor(w.x, w.y, w.z));
	lineX = gaugeWidth * 0.50f + posSX;
	lineY = gaugeHeight * 0.50f + posSY;
	DrawLine(lineX, lineY, lineX, posEY, GetColor(w.x, w.y, w.z));
	lineX = gaugeWidth * 0.75f + posSX;
	lineY = gaugeHeight * 0.75f + posSY;
	DrawLine(lineX, lineY, lineX, posEY, GetColor(w.x, w.y, w.z));

	// 画像描画位置計算
	float graphX = GameConst::WINDOW_WIDTH * GRAPH_WIDTH_POS_RATIO;
	float graphY = GameConst::WINDOW_HEIGHT * GRAPH_HEIGHT_POS_RATIO;
	
	// 画像描画
	DrawGraph(graphX, graphY, resistTimeGraphHandle, TRUE);
}

/*
 *	グラフハンドルのセット
 */
void ResistTimeGaugeUI::SetGraphHandle(int setHandle) {
	resistTimeGraphHandle = setHandle;
}
