/*
 *	@file	AudioManager.cpp
 *	@author	Seki
 */

#include "ActionManager.h"

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
	isActive = true;
	// 現在のアクショの生成
	currentAction = ActionFactory::CreateAction(ActionFactory::ActionType::Dungeon);
	if (!currentAction) return;
	// アクションのダウンキャスト
	auto dungeonAction = std::dynamic_pointer_cast<ActionDungeon>(currentAction);
	if (!dungeonAction) return;

	dungeonAction->SetDungeonStageData(setStageData);
	currentAction = dungeonAction;
	currentAction->Initialize(engine);
}

void ActionManager::DebugActiveDungeon(Engine& engine, DungeonStageData setStageData) {
	isActive = true;
	// 現在のアクショの生成
	currentAction = ActionFactory::CreateAction(ActionFactory::ActionType::Dungeon);
	if (!currentAction) return;
	// アクションのダウンキャスト
	auto dungeonAction = std::dynamic_pointer_cast<ActionDungeon>(currentAction);
	if (!dungeonAction) return;

	dungeonAction->DebugInitialize(engine, setStageData);
	currentAction = dungeonAction;
}

void ActionManager::ActiveTraining(Engine& engine) {
	isActive = true;
	currentAction = ActionFactory::CreateAction(ActionFactory::ActionType::Training);
	if (!currentAction) return;

	currentAction->Initialize(engine);
}

void ActionManager::ActiveShop(Engine& engine) {
	isActive = true;
	currentAction = ActionFactory::CreateAction(ActionFactory::ActionType::Shop);
	if (!currentAction) return;

	currentAction->Initialize(engine);	currentAction->Initialize(engine);
}

void ActionManager::ActivePartTime(Engine& engine) {
	isActive = true;
	currentAction = ActionFactory::CreateAction(ActionFactory::ActionType::PartTime);
	if (!currentAction) return;

	currentAction->Initialize(engine);	currentAction->Initialize(engine);
}
