/*
 *	@file	AudioManager.cpp
 *	@author	Seki
 */

#include "ActionManager.h"
#include "ActionDungeon/ActionDungeon.h"

/*
 *	更新処理
 */
void ActionManager::Update(Engine& engine, float deltaTime) {
	if (!isActive || !currentAction) return;

	currentAction->Update(engine, deltaTime);

	if (currentAction->IsComplete()) {
		onComplete();
		currentAction = nullptr;
		isActive = false;
	}
}

void ActionManager::Render() {
	if (!isActive || !currentAction) return;

	currentAction->Render();
}

/*
 *	ダンジョンアクション開始
 */
void ActionManager::ActiveDungeon(Engine& engine, DungeonStageData setStageData) {
	auto dungeonAction = std::dynamic_pointer_cast<ActionDungeon>(actionBase);
	if (!dungeonAction) return;

	dungeonAction->SetDungeonStageData(setStageData);
	currentAction = dungeonAction;
	currentAction->Initialize(engine);
}

void ActionManager::DebugActiveDungeon(Engine& engine, DungeonStageData setStageData) {
	isActive = true;
	auto dungeonAction = std::dynamic_pointer_cast<ActionDungeon>(actionBase);
	if (!dungeonAction) return;
	
	dungeonAction->DebugInitialize(engine, setStageData);
	currentAction = dungeonAction;
}

void ActionManager::ActiveTraining(Engine& engine) {
	currentAction = std::dynamic_pointer_cast<ActionDungeon>(actionBase);
	currentAction->Initialize(engine);
}

void ActionManager::ActiveShop(Engine& engine) {
	currentAction = std::dynamic_pointer_cast<ActionDungeon>(actionBase);
	currentAction->Initialize(engine);
}

void ActionManager::ActivePartTime(Engine& engine) {
	currentAction = std::dynamic_pointer_cast<ActionDungeon>(actionBase);
	currentAction->Initialize(engine);
}
