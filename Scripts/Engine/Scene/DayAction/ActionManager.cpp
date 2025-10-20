#include "ActionManager.h"
#include "ActionDungeon/ActionDungeon.h"

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

void ActionManager::DebugActiveDungeon(Engine& engine, std::string setFilePath) {
	auto dungeonAction = std::dynamic_pointer_cast<ActionDungeon>(actionBase);
	if (!dungeonAction) return;
	
	dungeonAction->DebugInitialize(engine, setFilePath);
	currentAction = dungeonAction;
	isActive = true;
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
