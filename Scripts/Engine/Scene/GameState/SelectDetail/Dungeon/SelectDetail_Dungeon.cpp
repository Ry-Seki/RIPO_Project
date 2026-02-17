/*
 *	@file	SelectDetail_Dungeon.cpp
 *	@author	Seki
 */

#include "SelectDetail_Dungeon.h"
#include "../SelectDetailBase.h"
#include "../GameState_SelectDetail.h"
#include "../../GameStateMachine.h"
#include "../../../../Load/LoadManager.h"
#include "../../../../Load/Dungeon/DungeonDataLoader.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../Audio/AudioUtility.h"
#include "../../../../Load/JSON/LoadJSON.h"
#include "../../../../JSON.h"
#include "../../../../../Data/Dungeon/DungeonStageData.h"
#include "../../../../../Data/Dungeon/DungeonFloorData.h"
#include "../../../../Menu/MainGame/Dungeon/MenuSelectDungeon.h"
#include "../../../../Menu/MenuManager.h"
#include "../../../../System/Money/MoneyManager.h"

/*
 *	@brief	初期化処理
 */
void SelectDetail_Dungeon::Initialize() {
	auto& menu = MenuManager::GetInstance();
	auto dungeonMenu = menu.GetMenu<MenuSelectDungeon>();
	dungeonMenu->SetCallback([this](int dungeonID) {
		StartDungeonDataLoad(dungeonID);
	});

	LoadManager& load = LoadManager::GetInstance();
	dungeonDataLoader = load.LoadResource<DungeonDataLoader>(_DUNGEON_LIST_PATH);
	load.SetOnComplete([this]() { SetupData(); });
}
/*
 *	@brief	ロード済みデータのセット
 */
void SelectDetail_Dungeon::SetupData() {
	dungeonDataList = dungeonDataLoader->dungeonList;
}
/*
 *	@brief	準備前処理
 */
void SelectDetail_Dungeon::Setup() {
	auto& menu = MenuManager::GetInstance();
	AssessmentTreasureEvent();
	auto dungeonMenu = menu.GetMenu<MenuSelectDungeon>();
	dungeonMenu->SetInfoData(ToDungeonInfoData());
	menu.OpenMenu<MenuSelectDungeon>();
}
/*
 *	@brief	更新処理
 */
void SelectDetail_Dungeon::Update(float deltaTime) {
	if (dungeonDataList.empty()) return;

	if (!inputHandle && CheckHitKey(KEY_INPUT_1)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		dungeonID = 1;
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			StartDungeonDataLoad(dungeonID);
		});
	}
	if (!inputHandle && CheckHitKey(KEY_INPUT_2)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		dungeonID = 2;
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			StartDungeonDataLoad(dungeonID);
		});
	}
	if (!inputHandle && CheckHitKey(KEY_INPUT_3)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		dungeonID = 3;
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			StartDungeonDataLoad(dungeonID);
											 });
	}
	if (!inputHandle && CheckHitKey(KEY_INPUT_4)) {
		// SEの再生
		AudioUtility::PlaySE("DebugSE");
		dungeonID = 4;
		inputHandle = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			StartDungeonDataLoad(dungeonID);
											 });
	}
	if (CheckHitKey(KEY_INPUT_1) == 0) inputHandle = false;
}
/*
 *	@brief	描画処理
 */
void SelectDetail_Dungeon::Render() {
}
/*
 *	@brief	片付け処理
 */
void SelectDetail_Dungeon::Teardown() {
	inputHandle = false;
}
/*
 *	@brief	お宝イベント査定
 */
void SelectDetail_Dungeon::AssessmentTreasureEvent() {
	auto& context = owner->GetOwner()->GetActionContext();
	// 経過時間を取得
	int elapsedDay = context.elapsedDay;
	// ダンジョンデータのイベント開始日と照らし合わせる
	for (int i = 0, max = dungeonDataList.size(); i < max; i++) {
		DungeonData data = dungeonDataList[i];
		if (elapsedDay >= data.eventStartDay && elapsedDay <= data.eventEndDay) {
			if (!data.isEventDay) data.isEventDay = true;

		} else {
			if (data.isEventDay) data.isEventDay = false;
		}
		dungeonDataList[i] = data;
	}
}
/*
 *	@brief	ダンジョンステージデータの読み込み
 */
void SelectDetail_Dungeon::StartDungeonDataLoad(int dungeonID) {
	this->dungeonID = dungeonID;
	LoadManager& load = LoadManager::GetInstance();
	DungeonData dungeonData = dungeonDataList[dungeonID];
	// ファイルパスの取得
	std::string filePath = dungeonData.dungeonPath;
	if (filePath.empty()) return;
	// ダンジョンステージデータの読み込み
	std::vector<std::shared_ptr<LoadJSON>> jsonList;
	jsonList.push_back(load.LoadResource<LoadJSON>(filePath));
	jsonList.push_back(load.LoadResource<LoadJSON>(_DUNGEON_FLOOR_PATH));
	// コールバック登録
	load.SetOnComplete([this, jsonList]() {SetDungeonData(jsonList); });
}
/*
 *	@brief		読み込んだステージデータをセットし、アクションマネージャーに渡す(コールバック)
 *  @param[in]	const std::vector<std::shared_ptr<LoadJSON>>& setDataList	ロードしたJSONデータリスト
 */
void SelectDetail_Dungeon::SetDungeonData(const std::vector<std::shared_ptr<LoadJSON>>& setDataList) {
	// JSONデータの取得
	JSON dungeonData = setDataList[0]->GetData();
	JSON dungeonfloorData = setDataList[1]->GetData();
	if (dungeonData.empty() || dungeonfloorData.empty()) return;

	auto& context = owner->GetOwner()->GetActionContext();
	// それぞれのデータを初期化
	context.dungeonID = dungeonID;
	context.prevIncome = MoneyManager::GetInstance().GetCurrentMoney();
	context.isCurrentEvent = dungeonDataList[dungeonID].isEventDay;
	context.dungeonStageData.LoadFromJSON(dungeonData);
	context.dungeonFloorData.LoadFromJSON(dungeonfloorData, dungeonID);
	// ステートの切り替え
	owner->GetOwner()->ChageState(GameEnum::GameState::InAction);
}
/*
 *	@brief		ダンジョンデータからダンジョン情報データへ変換
 *	@return		std::vector<DungeonInfoData>
 */
std::vector<DungeonInfoData> SelectDetail_Dungeon::ToDungeonInfoData() {
	std::vector<DungeonInfoData> dataList;
	for (int i = 1, max = dungeonDataList.size(); i < max; i++) {
		DungeonInfoData data{};
		data.isEventDay = dungeonDataList[i].isEventDay;
		data.eventStartDay = dungeonDataList[i].eventStartDay;
		data.eventEndDay = dungeonDataList[i].eventEndDay;
		data.levelOfDanger = dungeonDataList[i].levelOfDanger;
		data.necessaryStrength = dungeonDataList[i].necessaryStrength;
		data.treasureCount = dungeonDataList[i].treasureCount;
		dataList.push_back(data);
	}
	return dataList;
}
