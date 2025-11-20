/*
 *	@file	ActionPartTime.cpp
 *	@author	Seki
 */

#include "ActionPartTime.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"
#include "../../../System/Money/MoneyManager.h"

/*
 *	初期化処理
 */
void ActionPartTime::Initialize(Engine& engine) {
	auto fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		MoneyManager::GetInstance().AddMoney(incomeValue);
	});
}
/*
 *  ロード済みのデータをセット(コールバック)
 */
void ActionPartTime::SetupData(Engine& engine) {

}
/*
 *	更新処理
 */
void ActionPartTime::Update(Engine& engine, float deltaTime) {
	timer += deltaTime;
	if (timer >= limitTime) {
		isComplete = true;
	}
}
/*
 *	描画処理
 */

void ActionPartTime::Render() {

}
/*
 *	破棄処理
 */
void ActionPartTime::Teardown() {

}
