/*
 *	@file	AudioManager.cpp
 *	@author	Seki
 */

#include "ActionManager.h"
#include "DayActionBase.h"
#include "ActionFactory.h"
#include "ActionDungeon/ActionDungeon.h"
#include "ActionTraining/ActionTraining.h"
#include "ActionShop/ActionShop.h"
#include "ActionPartTime/ActionPartTime.h"

#include "../../Engine.h"

/*
 *	@brief	初期化処理
 */
void ActionManager::Initialize(JSON setJSON) {
}
/*
 *	@brief	更新処理
 */
void ActionManager::Update(Engine& engine, float deltaTime) {
	if (!isActive || !currentAction) return;

	currentAction->Update(engine, deltaTime);

	if (currentAction->IsComplete()) {
		onComplete();
		currentAction->Teardown();
		currentAction = nullptr;
		isActive = false;
	}
}
/*
 *	@brief	描画処理
 */
void ActionManager::Render() {
	if (!isActive || !currentAction) return;

	currentAction->Render();
}
/*
 *	@brief	ダンジョンアクション開始
 */
void ActionManager::ActiveDungeon(Engine& engine, DungeonStageData setStageData, DungeonFloorData setFloorData) {
	currentAction = ActionFactory::CreateAction(GameEnum::ActionType::Dungeon);
	if (!currentAction) return;

	auto dungeonAction = std::dynamic_pointer_cast<ActionDungeon>(currentAction);
	if (!dungeonAction) return;

	dungeonAction->SetDungeonStageData(setStageData);
	currentAction = dungeonAction;
	currentAction->Initialize(engine);
	isActive = true;
}
/*
 *	@brief	デバッグ用ダンジョンアクション開始
 */
void ActionManager::DebugActiveDungeon(Engine& engine, DungeonStageData setStageData, DungeonFloorData setFloorData) {
	currentAction = ActionFactory::CreateAction(GameEnum::ActionType::Dungeon);
	if (!currentAction) return;

	auto dungeonAction = std::dynamic_pointer_cast<ActionDungeon>(currentAction);
	if (!dungeonAction) return;

	dungeonAction->DebugInitialize(engine, setStageData, setFloorData);
	currentAction = dungeonAction;
	isActive = true;
}
/*
 *  @brief		トレーニングアクション開始
 *  @param[in]  const int setActionNum    アクション番号
 */
void ActionManager::ActiveTraining(Engine& engine, const int setActionNum) {
	currentAction = ActionFactory::CreateAction(GameEnum::ActionType::Training);
	if (!currentAction) return;

	auto trainingAction = std::dynamic_pointer_cast<ActionTraining>(currentAction);
	if (!trainingAction) return;

	trainingAction->SetActionNum(setActionNum);
	currentAction = trainingAction;
	currentAction->Initialize(engine);
	isActive = true;
}
/*
 *  @brief		ショップアクション開始
 *  @param[in]  const std::vector<int>& itemIDList     アイテムIDリスト
 */
void ActionManager::ActiveShop(Engine& engine, const std::vector<int>& itemIDList) {
	currentAction = ActionFactory::CreateAction(GameEnum::ActionType::Shop);
	if (!currentAction) return;

	auto shopAction = std::dynamic_pointer_cast<ActionShop>(currentAction);
	if (!shopAction) return;

	shopAction->SetItemIDList(itemIDList);
	currentAction = shopAction;
	currentAction->Initialize(engine);
	isActive = true;
}
/*
 *	@brief		アルバイトアクション開始
 *  @param[in]	const int incomeValue		アルバイトの収入
 */
void ActionManager::ActivePartTime(Engine& engine, const int incomeValue) {
	currentAction = ActionFactory::CreateAction(GameEnum::ActionType::PartTime);
	if (!currentAction) return;

	auto partTimeAction = std::dynamic_pointer_cast<ActionPartTime>(currentAction);
	if (!partTimeAction) return;

	partTimeAction->SetIncomeValue(incomeValue);
	currentAction = partTimeAction;
	currentAction->Initialize(engine);
	isActive = true;
}
/*
 *  @brief      行動クラスの取得
 *  @param[in]  ActionType type
 *  @return     std::shared_ptr<DayActionBase>
 */
std::shared_ptr<DayActionBase> ActionManager::GetAction(GameEnum::ActionType type,Engine& engine) {
	auto action = actionMap[type];
	if (!action) {
		action = ActionFactory::CreateAction(type);
		action->Initialize(engine);
		actionMap[type] = action;
	}
	return action;
}
