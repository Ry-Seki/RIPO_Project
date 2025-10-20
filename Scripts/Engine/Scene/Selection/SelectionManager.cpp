/*
 *	@file	SelectionManager.cpp
 *	@author	Seki
 */

#include "SelectionManager.h"
#include "Dungeon/SelectionDungeon.h"

void SelectionManager::Update(float deltaTime) {
	if (currentSelection) currentSelection->Update(deltaTime);
}

void SelectionManager::Render() {
	if (currentSelection) currentSelection->Render();
}

void SelectionManager::DungeonSelection() {
	auto dungeonSelection = std::dynamic_pointer_cast<SelectionDungeon>(selectionBase);
	if (!dungeonSelection) return;

	currentSelection = dungeonSelection;
	currentSelection->Initialize();
}

void SelectionManager::TrainingSelection() {
}

void SelectionManager::ShopSelection() {
}

void SelectionManager::PartTimeSelection() {
}
