/*
 *  @file   MainGameScene.h
 */

#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_
#include "Scene.h"
#include "Calendar/CalendarManager.h"

#include <memory>
#include <DxLib.h>

class MainGameScene : public Scene {
private:
    std::shared_ptr<CalendarSystem> calendarSystem;
    std::shared_ptr<CalendarManager> calendarManager;

public:
    void Initialize(class Engine& engine) override;
    void Update(class Engine& engine, float deltaTime) override;
    void Render() override;
};
#endif // !_MAINGAMESCENE_H_



