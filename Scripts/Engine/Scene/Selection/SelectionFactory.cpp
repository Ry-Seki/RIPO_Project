/*
 *  @file   SelectionFactory.cpp
 *  @author Seki
 */

#include "SelectionFactory.h"
#include "Training/SelectionTraining.h"
#include "Shop/SelectionShop.h"
#include "PartTime/SelectionPartTime.h"

// 静的メンバ変数の初期化
std::unordered_map<GameEnum::ActionType, SelectionFactory::CreateFuntion>
SelectionFactory::selectionMap;

/*
 *	@brief		選択クラス登録
 *  @param[in]	ActionType type
 *  @param[in]	CreateFuntion function
 */
void SelectionFactory::RegisterAll() {
    Register(GameEnum::ActionType::Training, []() { return std::make_shared<SelectionTraining>(); });
    Register(GameEnum::ActionType::Shop,     []() { return std::make_shared<SelectionShop>(); });
    Register(GameEnum::ActionType::PartTime, []() { return std::make_shared<SelectionPartTime>(); });
}
/*
 *	@brief		選択クラスを全て登録
 */
void SelectionFactory::Register(GameEnum::ActionType type, CreateFuntion function) {
    selectionMap[type] = function;
}
/*
 *	@brief		選択クラス生成
 *  @param[in]	ActionType type
 *  @return		std::shared_ptr<SelectionBase>
 */
std::shared_ptr<SelectionBase> SelectionFactory::CreateSelection(GameEnum::ActionType type) {
    auto itr = selectionMap.find(type);
    if (itr == selectionMap.end()) return nullptr;

    return itr->second();
}
