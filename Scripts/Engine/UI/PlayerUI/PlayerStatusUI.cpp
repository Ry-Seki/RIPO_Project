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
	float graphPosX = GameConst::WINDOW_WIDTH * 0.02f;
	float posX = GameConst::WINDOW_WIDTH * 0.1f;
	float posY = GameConst::WINDOW_HEIGHT * 0.05f;
	FontManager::GetInstance().Draw("playerStatus", posX, posY, hp + " / " + hpMax, GetColor(0, 255, 0));
	DrawGraph(graphPosX, posY, HPGraphHandle, TRUE);
	posY = GameConst::WINDOW_HEIGHT * 0.15f;
	FontManager::GetInstance().Draw("playerStatus", posX, posY, stamina + " / " + staminaMax, GetColor(255, 255, 0));
	DrawGraph(graphPosX, posY, StaminaGraphHandle, TRUE);
	posY = GameConst::WINDOW_HEIGHT * 0.25f;
	FontManager::GetInstance().Draw("playerStatus", posX, posY, strength, GetColor(255, 0, 0));
	DrawGraph(graphPosX, posY, StrengthGraphHandle, TRUE);

}

/*
 *	グラフハンドルのセット
 */
void PlayerStatusUI::SetGraphHandle(int setHPHandle, int setStaminaHandle, int setStrengthHandle) {
	HPGraphHandle = setHPHandle;
	StaminaGraphHandle = setStaminaHandle;
	StrengthGraphHandle = setStrengthHandle;
}
