/*
 *	@file	DungeonMoneyUI.h
 *	@author	kuu
 */
#include "DungeonMoneyUI.h"
#include "../../Manager/FontManager.h"
#include "../../System/Money/MoneyManager.h"

void DungeonMoneyUI::Initialize()
{

}

void DungeonMoneyUI::Render()
{
	auto& money = MoneyManager::GetInstance();
	auto& font = FontManager::GetInstance();
	std::string showMoneyStr = std::to_string(money.GetCurrentMoney());
	font.Draw("NormalSizeFont", 1550, 100, showMoneyStr, GetColor(255, 255, 255));
}
