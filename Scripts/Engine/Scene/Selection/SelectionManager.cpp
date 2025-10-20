/*
 *	@file	SelectionManager.cpp
 *	@author	Seki
 */

#include "SelectionManager.h"
#include "Dungeon/SelectionDungeon.h"
#include "../../Engine.h"

void SelectionManager::Update(Engine& engine, float deltaTime) {
	if (!isActive) return;

	if (currentSelection) currentSelection->Update(engine, deltaTime);
}

void SelectionManager::Render() {
	if (!isActive) return;

	if (currentSelection) currentSelection->Render();
}

void SelectionManager::DungeonSelection(Engine& engine) {
	auto dungeonSelection = std::dynamic_pointer_cast<SelectionDungeon>(selectionBase);
	if (!dungeonSelection) return;

	currentSelection = dungeonSelection;
	currentSelection->Initialize(engine);
	isActive = true;
}

void SelectionManager::TrainingSelection() {
}

void SelectionManager::ShopSelection() {
}

void SelectionManager::PartTimeSelection() {
}
