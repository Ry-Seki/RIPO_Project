/*
 *  @file   ActionFactory.cpp
 */
#include "ActionFactory.h"
#include "ActionDungeon/ActionDungeon.h"
#include "ActionTraining/ActionTraining.h"
#include "ActionShop/ActionShop.h"
#include "ActionPartTime/ActionPartTime.h"

 // 静的メンバ変数の初期化
std::unordered_map<GameEnum::ActionType, ActionFactory::CreateFuntion>
ActionFactory::actionMap;

/*
 *	@brief		行動クラスを全て登録
 */
void ActionFactory::RegisterAll() {
    Register(GameEnum::ActionType::Dungeon,  []() { return std::make_shared<ActionDungeon>(); });
    Register(GameEnum::ActionType::Training, []() { return std::make_shared<ActionTraining>(); });
    Register(GameEnum::ActionType::Shop,     []() { return std::make_shared<ActionShop>(); });
    Register(GameEnum::ActionType::PartTime, []() { return std::make_shared<ActionPartTime>(); });
}
/*
 *	@brief		行動クラス登録
 *  @param[in]	ActionType type
 *  @param[in]	CreateFuntion function
 */
void ActionFactory::Register(GameEnum::ActionType type, CreateFuntion function) {
    actionMap[type] = function;
}
/*
 *	@brief		行動クラス生成
 *  @param[in]	ActionType type
 *  @return		std::shared_ptr<SelectionBase>
 */
std::shared_ptr<DayActionBase> ActionFactory::CreateAction(GameEnum::ActionType type) {
    auto itr = actionMap.find(type);
    if (itr == actionMap.end()) return nullptr;

    return itr->second();
}
