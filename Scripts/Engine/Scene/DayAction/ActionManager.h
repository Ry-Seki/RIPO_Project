/*
 *	@file	ActionManager.h
 *	@author	Seki
 */

#ifndef _ACTION_MANAGER_H_
#define _ACTION_MANAGER_H_

#include "DayActionBase.h"
#include <vector>
#include <memory>
#include <string>

class ActionManager {
private:
    std::vector<DayActionPtr> actionList;
    DayActionPtr currentAction;

public:
    void AddAction(const DayActionPtr& action) {
        actionList.push_back(action);
        action->Initialize(); // LoadManager ‚Éƒtƒ@ƒCƒ‹“o˜^
    }

    void SetupAction(const std::string& name) {
        for (auto& action : actionList) {
            if (action->GetName() == name) {
                action->Setup();
                currentAction = action;
                break;
            }
        }
    }

    void Update(float deltaTime) {
        if (currentAction) currentAction->Update(deltaTime);
    }
};

#endif // !_ACTION_MANAGER_H_
