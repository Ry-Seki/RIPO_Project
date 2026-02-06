/*
 *	@file	ResultScene.cpp
 *	@author	Seki
 */

#include "ResultScene.h"
#include "TitleScene.h"
#include "../Engine.h"
#include "../Fade/FadeFactory.h"
#include "../Fade/FadeManager.h"
#include "../System/Money/MoneyManager.h"
#include "../Load/LoadManager.h"
#include "../Load/JSON/LoadJSON.h"
#include "../Menu/MenuManager.h"
#include "../Menu/Result/MenuResultScore.h"
#include "../../Data/Result/ResultScoreData.h"

#include <DxLib.h>	

/*
 *	@brief	初期化処理
 */
void ResultScene::Initialize(Engine& engine) {
	MenuManager::GetInstance().GetMenu<MenuResultScore>();
	auto& load = LoadManager::GetInstance();
	auto resultScore = load.LoadResource<LoadJSON>(_RESULT_SCORE_PATH);
	load.SetOnComplete([this, &engine, resultScore]() {
		SetupData(engine, resultScore->GetData());
	});
}
/*
 *	@brief	ロード済みデータのセット (コールバック)
 */
void ResultScene::SetupData(Engine& engine, const JSON& json) {
    auto& menu = MenuManager::GetInstance();
    auto result = menu.GetMenu<MenuResultScore>();
    ResultScoreData data = ToScoreData(json);
    result->SetResultScoreData(data);
    Setup(engine);
}
/*
 *	@brief	準備前処理
 */
void ResultScene::Setup(Engine& engine) {
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade, [this]() {
		isStart = true;
		MenuManager::GetInstance().OpenMenu<MenuResultScore>();
	});
}
/*
 *	@brief	更新処理
 */
void ResultScene::Update(Engine& engine, float deltaTime) {
	if (!isStart) return;

	
}
/*
 *	@brief	描画処理
 */
void ResultScene::Render() {
	MoneyManager& money = MoneyManager::GetInstance();
	DrawFormatString(50, 50, GetColor(255, 255, 255), "ResultMoney : %d", money.GetCurrentMoney());
	DrawFormatString(50, 70, GetColor(255, 255, 255), "ResultScore ");
}
/*
 *	@brief		読み込んだJSONデータを構造体に変換
 *  @param[in]	const JSON& json
 *	@return		ResultScoreData
 */
ResultScoreData ResultScene::ToScoreData(const JSON& json) {
    ResultScoreData data;

    // enum 数に合わせて確保
    data.requiredScore.resize(static_cast<int>(GameEnum::ResultRank::Max), 0);
    // 文字列と列挙体をむずびつけるための処置
    struct RankKey {
        GameEnum::ResultRank rank;
        const char* key;
    };
    static const RankKey rankTable[] = {
        { GameEnum::ResultRank::S, "S" },
        { GameEnum::ResultRank::A, "A" },
        { GameEnum::ResultRank::B, "B" },
        { GameEnum::ResultRank::C, "C" },
        { GameEnum::ResultRank::D, "D" },
    };

    for (const auto& entry : rankTable) {
        const int index = static_cast<int>(entry.rank);

        data.requiredScore[index] = json.value(entry.key, 0);
    }
    return data;
}
