/*
 *	@file	SelectionManager.cpp
 *	@author	Seki
 */

#include "SelectionManager.h"
#include "SelectionBase.h"
#include "SelectionFactory.h"
#include "Dungeon/SelectionDungeon.h"
#include "Training/SelectionTraining.h"
#include "Shop/SelectionShop.h"
#include "PartTime/SelectionPartTime.h"
#include "../DayAction/ActionManager.h"

#include "../../Engine.h"
#include "../../GameEnum.h"

/*
 *	更新処理
 */
void SelectionManager::Update(Engine& engine, float deltaTime) {
	if (!currentSelection || !isActive) return;

	currentSelection->Update(engine, deltaTime);

	if (currentSelection->IsComplete()) {
		isActive = false;
		prevSelection = currentSelection;
		currentSelection = nullptr;
	}
}
/*
 *	描画処理
 */
void SelectionManager::Render() {
	if (!isActive) return;

	if (currentSelection) currentSelection->Render();
}
/*
 *	ダンジョン選択
 */
void SelectionManager::DungeonSelection(Engine& engine) {
	currentSelection = SelectionFactory::CreateSelection(GameEnum::ActionType::Dungeon);
	if (!currentSelection) return;

	auto dungeonSelection = std::dynamic_pointer_cast<SelectionDungeon>(currentSelection);
	if (!dungeonSelection) return;

	dungeonSelection->SetActiveDungeon([this](Engine& engine, DungeonStageData setStageData, DungeonFloorData setFloorData) {
		actionManager->DebugActiveDungeon(engine, setStageData, setFloorData);
	});
	currentSelection = dungeonSelection;
	currentSelection->Initialize(engine);
	isActive = true;
}
/*
 *	トレーニング選択
 */
void SelectionManager::TrainingSelection(Engine& engine) {
	currentSelection = SelectionFactory::CreateSelection(GameEnum::ActionType::Training);
	if (!currentSelection) return;

	auto trainingSelection = std::dynamic_pointer_cast<SelectionTraining>(currentSelection);
	if (!trainingSelection) return;

	trainingSelection->SetActiveTraining([this](Engine& engine, int setActionNum) {
		actionManager->ActiveTraining(engine, setActionNum);
	});
	currentSelection = trainingSelection;
	currentSelection->Initialize(engine);
	isActive = true;
}
/*
 *	ショップ選択
 */
void SelectionManager::ShopSelection(Engine& engine) {
	currentSelection = SelectionFactory::CreateSelection(GameEnum::ActionType::Shop);
	if (!currentSelection) return;

	auto shopSelection = std::dynamic_pointer_cast<SelectionShop>(currentSelection);
	if (!shopSelection) return;

	shopSelection->SetActiveShop([this](Engine& engine, std::vector<int> itemIDList) {
		actionManager->ActiveShop(engine, itemIDList);
	});
	currentSelection = shopSelection;
	currentSelection->Initialize(engine);
	isActive = true;
}
/*
 *	アルバイト選択
 */
void SelectionManager::PartTimeSelection(Engine& engine) {
	currentSelection = SelectionFactory::CreateSelection(GameEnum::ActionType::PartTime);
	if (!currentSelection) return;

	auto partTimeSelection = std::dynamic_pointer_cast<SelectionPartTime>(currentSelection);
	if (!partTimeSelection) return;

	partTimeSelection->SetActivePartTime([this](Engine& engine, int incomeValue) {
		actionManager->ActivePartTime(engine, incomeValue);
	});
	currentSelection = partTimeSelection;
	currentSelection->Initialize(engine);
	isActive = true;
}
/*
 *	@brief		選択クラスの取得
 *  @param[in]	ActionType type
 *	@return		std::shared_ptr<SelectionBase>
 */
std::shared_ptr<SelectionBase> SelectionManager::GetSelection(GameEnum::ActionType type, Engine& engine) {
	auto& selection = selectionMap[type];
	// 取得できなければ生成
	if (!selection) {
		selection = SelectionFactory::CreateSelection(type);
		selection->Initialize(engine);
		selectionMap[type] = selection;
	}
	return selection;
}
