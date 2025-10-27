/*
 *	@file	SelectionManager.cpp
 *	@author	Seki
 */

#include "SelectionManager.h"
#include "Dungeon/SelectionDungeon.h"
#include "../../Engine.h"

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
	auto dungeonSelection = std::dynamic_pointer_cast<SelectionDungeon>(selectionBase);
	if (!dungeonSelection) return;

	dungeonSelection->SetActiveDungeon([this](Engine& engine, DungeonStageData setStageData) {
		actionManager->DebugActiveDungeon(engine, setStageData);
	});
	currentSelection = dungeonSelection;
	currentSelection->Initialize(engine);
	isActive = true;
}
/*
 *	トレーニング選択
 */
void SelectionManager::TrainingSelection() {
}
/*
 *	ショップ選択
 */
void SelectionManager::ShopSelection() {
}
/*
 *	アルバイト選択
 */
void SelectionManager::PartTimeSelection() {
}
