/*
 *	@file	AudioManager.cpp
 *	@author	Seki
 */

#include "ActionManager.h"

 /*
  *	初期化処理
  */
void ActionManager::Initialize(JSON setJSON) {
}
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
/*
 *	描画処理
 */
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
	currentAction = ActionFactory::CreateAction(ActionType::Dungeon);
	if (!currentAction) return;
	// アクションのダウンキャスト
	auto dungeonAction = std::dynamic_pointer_cast<ActionDungeon>(currentAction);
	if (!dungeonAction) return;

	dungeonAction->SetDungeonStageData(setStageData);
	currentAction = dungeonAction;
	currentAction->Initialize(engine);
}
/*
 *	デバッグ用ダンジョンアクション開始
 */
void ActionManager::DebugActiveDungeon(Engine& engine, DungeonStageData setStageData) {
	isActive = true;
	// 現在のアクショの生成
	currentAction = ActionFactory::CreateAction(ActionType::Dungeon);
	if (!currentAction) return;
	// アクションのダウンキャスト
	auto dungeonAction = std::dynamic_pointer_cast<ActionDungeon>(currentAction);
	if (!dungeonAction) return;

	dungeonAction->DebugInitialize(engine, setStageData);
	currentAction = dungeonAction;
}
/*
 *	トレーニングアクション開始
 */
void ActionManager::ActiveTraining(Engine& engine, int setActionNum) {
	isActive = true;
	currentAction = ActionFactory::CreateAction(ActionType::Training);
	if (!currentAction) return;
	auto trainingAction = std::dynamic_pointer_cast<ActionTraining>(currentAction);
	if (!trainingAction) return;

	trainingAction->SetActionNum(setActionNum);
	currentAction = trainingAction;
	currentAction->Initialize(engine);
}
/*
 *	ショップアクション開始
 */
void ActionManager::ActiveShop(Engine& engine) {
	isActive = true;
	currentAction = ActionFactory::CreateAction(ActionType::Shop);
	if (!currentAction) return;

	currentAction->Initialize(engine);	currentAction->Initialize(engine);
}
/*
 *	アルバイトアクション開始
 */
void ActionManager::ActivePartTime(Engine& engine) {
	isActive = true;
	currentAction = ActionFactory::CreateAction(ActionType::PartTime);
	if (!currentAction) return;

	currentAction->Initialize(engine);	currentAction->Initialize(engine);
}
