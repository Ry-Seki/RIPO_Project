/*
 *	@file	ActionManager.h
 *	@author	Seki
 */

#ifndef _ACTION_MANAGER_H_
#define _ACTION_MANAGER_H_

#include "../../Singleton.h"
#include "DayActionBase.h"


#include <vector>
#include <memory>
#include <string>

class ActionManager : public Singleton<ActionManager>{
    friend class Singleton<ActionManager>;

private:
    std::vector<DayActionPtr> actionList;
    DayActionPtr currentAction;

public:
    /*
     *  アクションの追加
     */
    void AddAction(const DayActionPtr& action) {
        actionList.push_back(action);
        action->Initialize(); // LoadManager にファイル登録
    }
    /*
     *  アクションの設定
     */
    void SetupAction(const std::string& name) {
        for (auto& action : actionList) {
            if (action->GetName() == name) {
                action->Setup();
                currentAction = action;
                break;
            }
        }
    }
    /*
     *  更新処理
     */
    void Update(float deltaTime) {
        if (currentAction) currentAction->Update(deltaTime);
    }
    /*
     *  描画処理
     */
    void Render() {
        if (currentAction) currentAction->Render();
    }
public:
    /*
     *  全てのアクションの ロード済みのデータをセット(コールバック)
     */
    void SetupAll() {
        for (auto& action : actionList) {
            action->Setup();
        }
        if (!actionList.empty()) currentAction = actionList.front();
    }
};

#endif // !_ACTION_MANAGER_H_
