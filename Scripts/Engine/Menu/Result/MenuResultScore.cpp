/*
 *	@file	MenuResultScore.cpp
 *	@author	Seki
 */

#include "MenuResultScore.h"
#include "../../Fade/FadeManager.h"
#include "../../Fade/FadeFactory.h"
#include "../../System/Money/MoneyManager.h"

/*
 *	@brief	初期化処理
 */
void MenuResultScore::Initialize(Engine& engine) {

}
/*
 *	@brief	メニューを開く
 */
void MenuResultScore::Open() {
    MenuBase::Open();
    int playerScore = MoneyManager::GetInstance().GetCurrentMoney();
    rank = JudgeRank(playerScore);
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        isStart = true;
    });

}
/*
 *	@brief	更新処理
 */
void MenuResultScore::Update(Engine& engine, float unscaledDeltaTime) {

}
/*
 *	@brief	描画処理
 */
void MenuResultScore::Render() {
    if (!isStart) return;

}
/*
 *	@brief	メニューを閉じる
 */
void MenuResultScore::Close(Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief	メニューを閉じる
 */
void MenuResultScore::Suspend() {
    MenuBase::Suspend();
}
/*
 *	@brief	メニューを再開
 */
void MenuResultScore::Resume() {
    MenuBase::Resume();
}
/*
 *	@brief		スコア->ランク判定
 *	@param[in]	int playerScore
 *  @return     GameEnum::ResultRank
 */
GameEnum::ResultRank MenuResultScore::JudgeRank(int playerScore) {
    for (int i = 0, max = static_cast<int>(GameEnum::ResultRank::Max); i < max; ++i) {
        if (playerScore >= resultScore.requiredScore[i]) {
            return static_cast<GameEnum::ResultRank>(i);
        }
    }
    return GameEnum::ResultRank::Invalid;
}
