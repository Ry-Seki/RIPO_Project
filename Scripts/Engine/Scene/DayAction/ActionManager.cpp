#include "ActionManager.h"
#include "ActionDungeon/ActionDungeon.h"

/*
 *	ダンジョンアクション開始
 */
void ActionManager::ActiveDungeon(DungeonStageData setStageData) {
	auto dungeonAction = std::dynamic_pointer_cast<ActionDungeon>(actionBase);
	if (!dungeonAction) return;
	dungeonAction->SetDungeonStageData(setStageData);

	currentAction = dungeonAction;
	currentAction->Initialize();
}

void ActionManager::ActiveTraining() {
	currentAction = actionList[(int)ActionType::TrainingAction];
	currentAction->Initialize();
}

void ActionManager::ActiveShop() {
	currentAction = actionList[(int)ActionType::ShopAction];
	currentAction->Initialize();
}

void ActionManager::ActivePartTime() {
	currentAction = actionList[(int)ActionType::PartTimeAction];
	currentAction->Initialize();
}
