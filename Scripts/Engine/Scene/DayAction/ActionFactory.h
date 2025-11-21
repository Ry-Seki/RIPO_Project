/*
 *	@file	ActionFactory.h
 *	@author	Seki
 */

#ifndef _ACTION_FACTORY_H_
#define _ACTION_FACTORY_H_

#include "DayActionBase.h"
#include "ActionDungeon/ActionDungeon.h"
#include "ActionTraining/ActionTraining.h"
#include "ActionShop/ActionShop.h"
#include "ActionPartTime/ActionPartTime.h"

#include <memory>
#include <string>

enum class ActionType {
    Dungeon,
    Training,
    Shop,
    PartTime
};

 /*
  *  各アクションの生成するファクトリクラス
  */
class ActionFactory {
public:
    /*
     *  アクション生成
     */
    static DayActionPtr CreateAction(ActionType type) {
        switch (type) {
            case ActionType::Dungeon: 
                return std::make_shared<ActionDungeon>();
            case ActionType::Training: 
                return std::make_shared<ActionTraining>();
            case ActionType::Shop: 
                return std::make_shared<ActionShop>();
            case ActionType::PartTime: 
                return std::make_shared<ActionPartTime>();
        }
    }
};

#endif // !_ACTION_FACTORY_H_
