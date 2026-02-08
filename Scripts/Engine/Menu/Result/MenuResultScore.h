/*
 *	@file	MenuResultScore.h
 *	@author	Seki
 */

#ifndef _MENU_RESULT_SCORE_H_
#define _MENU_RESULT_SCORE_H_

#include "../MenuBase.h"
#include "../../../Data/Result/ResultScoreData.h"
#include "../../GameEnum.h"
#include "../../UI/EventSystem.h"
#include "../../UI/Button/UIButtonBase.h"

// 前方宣言
class Engine;

/*
 *	@brief	リザルトスコアメニュー
 */
class MenuResultScore : public MenuBase {
private:
	int currentSlot = -1;
	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;

	ResultScoreData resultScore;
	GameEnum::ResultRank rank = GameEnum::ResultRank::Invalid;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/Result/ResultResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/Result/ResultNavigation.json";

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	メニューを開く
	 */
	void Open() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	メニューを閉じる
	 */
	void Close(Engine& engine) override;
	/*
	 *	@brief	メニューを中断
	 */
	void Suspend() override;
	/*
	 *	@brief	メニューを再開
	 */
	void Resume() override;

private:
	/*
	 *	@brief		ボタンの押された時の処理
	 *	@param[in]	int buttonIndex
	 */
	void SelectButtonExecute(Engine& engine, int buttonIndex);
	/*
	 *	@brief		スコア->ランク判定
	 *	@param[in]	int playerScore
	 *  @return     GameEnum::ResultRank
	 */
	GameEnum::ResultRank JudgeRank(int playerScore);

public:
	/*
	 *	@brief		スコアデータの設定
	 *	@param[in]	ResultScoreData
	 */
	inline void SetResultScoreData(ResultScoreData data) {
		resultScore = data;
	}
};

#endif // !_MENU_RESULT_SCORE_H_