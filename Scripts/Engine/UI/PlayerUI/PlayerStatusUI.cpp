/*
 *	@file	PlayerStatusUI.cpp
 *	@author Riku
 */

#include "PlayerStatusUI.h"
#include "../../Component/Character/CharacterUtility.h"
#include "../../System/Status/PlayerStatusManager.h"
#include "../../GameConst.h"
#include "../../Manager/FontManager.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"

using namespace CharacterUtility;

PlayerStatusUI::PlayerStatusUI() 
	: GRAPH_WIDTH_POS_RATIO(0.02f)
	, TEXT_WIDTH_POS_RATIO(0.1f)
	, HP_UI_HEIGHT_POS_RATIO(0.05f)
	, STAMINA_UI_HEIGHT_POS_RATIO(0.15f)
	, STRENGTH_UI_HEIGHT_POS_RATIO(0.25f)
{}

/*
 *	初期化処理
 */
void PlayerStatusUI::Initialize() {
	FontManager::GetInstance().LoadFont(80, "playerStatus");
	LoadManager& load = LoadManager::GetInstance();
	auto hpSprite = load.LoadResource<LoadSprite>(HP_PATH);
	auto staminaSprite = load.LoadResource<LoadSprite>(STAMINA_PATH);
	auto strengthSprite = load.LoadResource<LoadSprite>(STRENGTH_PATH);
	load.SetOnComplete([this, hpSprite, staminaSprite, strengthSprite]() {
		SetGraphHandle(hpSprite->GetHandle(), staminaSprite->GetHandle(), strengthSprite->GetHandle());
		});
}

/*
 *	描画処理
 */
void PlayerStatusUI::Render() {
	// 最大値と文字幅を合わせる
	auto playerStatus = GetPlayer()->GetComponent<PlayerComponent>()->GetPlayerStatus();
	auto baseStatus = PlayerStatusManager::GetInstance().GetPlayerStatusData().base;
	std::string hp = std::to_string(playerStatus.HP);
	std::string hpMax = std::to_string(baseStatus.HP);
	while (hp.size() < hpMax.size()) {
		hp = " " + hp;
	}
	std::string stamina = std::to_string(playerStatus.stamina);
	std::string staminaMax = std::to_string(baseStatus.stamina);
	while (stamina.size() < staminaMax.size()) {
		stamina = " " + stamina;
	}
	std::string strength = std::to_string(playerStatus.strength);
	
	// 3種画像とテキストの描画
	float graphPosX = GameConst::WINDOW_WIDTH * GRAPH_WIDTH_POS_RATIO;
	float textPosX = GameConst::WINDOW_WIDTH * TEXT_WIDTH_POS_RATIO;
	float posY = GameConst::WINDOW_HEIGHT * HP_UI_HEIGHT_POS_RATIO;

	// HPの画像とテキストの描画
	auto g = GameConst::COLOR_GREEN;
	FontManager::GetInstance().Draw("playerStatus", textPosX, posY, hp + " / " + hpMax, GetColor(g.x, g.y, g.z));
	DrawGraph(graphPosX, posY, HPGraphHandle, TRUE);
	
	// Y座標だけずらして
	posY = GameConst::WINDOW_HEIGHT * STAMINA_UI_HEIGHT_POS_RATIO;
	// スタミナの画像とテキストの描画
	auto y = GameConst::COLOR_YELLOW;
	FontManager::GetInstance().Draw("playerStatus", textPosX, posY, stamina + " / " + staminaMax, GetColor(y.x, y.y, y.z));
	DrawGraph(graphPosX, posY, staminaGraphHandle, TRUE);
	
	// Y座標だけずらして
	posY = GameConst::WINDOW_HEIGHT * STRENGTH_UI_HEIGHT_POS_RATIO;
	// ストレングスの画像とテキストの描画
	auto r = GameConst::COLOR_RED;
	FontManager::GetInstance().Draw("playerStatus", textPosX, posY, strength, GetColor(r.x, r.y, r.z));
	DrawGraph(graphPosX, posY, strengthGraphHandle, TRUE);

}

/*
 *	グラフハンドルのセット
 */
void PlayerStatusUI::SetGraphHandle(int setHPHandle, int setStaminaHandle, int setStrengthHandle) {
	HPGraphHandle = setHPHandle;
	staminaGraphHandle = setStaminaHandle;
	strengthGraphHandle = setStrengthHandle;
}
