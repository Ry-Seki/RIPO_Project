/*
 *	@file	DungeonMoneyUI.h
 *	@author	kuu
 */
#include "DungeonMoneyUI.h"
#include "../../System/Money/MoneyManager.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../GameConst.h"

void DungeonMoneyUI::Initialize()
{
	LoadManager& load = LoadManager::GetInstance();
	auto moneySprite = load.LoadResource<LoadSprite>(MONEY_PATH);
	load.SetOnComplete([this, moneySprite]() {
		MoneyGraphHandle = moneySprite->GetHandle();
		});
	displayTime = 0;
}

void DungeonMoneyUI::Update(float unscaledDeltaTime)
{
 	displayTime -= unscaledDeltaTime;
}

void DungeonMoneyUI::Render()
{
	// アイコンを表示
	float posX = GameConst::WINDOW_WIDTH * 0.72f;
	float posY = GameConst::WINDOW_HEIGHT * 0.06f;
	DrawGraph(posX, posY, MoneyGraphHandle, TRUE);
	// 現在の所持金を表示
	auto& money = MoneyManager::GetInstance();
	auto& font = FontManager::GetInstance();
	std::string showMoneyStr = std::to_string(money.GetCurrentMoney());
	font.Draw("NormalSizeFont", 1550, 100, showMoneyStr, GetColor(255, 255, 255));

	int addMoney = money.GetCurrentMoney() - money.GetPrevMoney();
	// 獲得した金の表示
	if (money.GetCurrentMoney() - prevMoney > 0) {
		displayTime = 2.0f;
	}
	// 一定時間表示
	if (displayTime > 0) {
		std::string addMoneyStr = "+" + std::to_string(addMoney);
		font.Draw("MiniSizeFont", 1550, 25, addMoneyStr, GetColor(255, 255, 255));
	}
	prevMoney = money.GetCurrentMoney();
}
