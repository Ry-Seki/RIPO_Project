/*
 *  @file   MainGameScene.h
 *  @author Seki
 */

#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_

#include "Scene.h"
#include "Calendar/CalendarManager.h"

#include <memory>
#include <DxLib.h>

/*
 *  メインゲームシーン
 */
class MainGameScene : public Scene {
private:
    std::unique_ptr<CalendarManager> calendarManager;   // カレンダー管理クラス

public:
    /*
     *  初期化処理
     */
    void Initialize(class Engine& engine) override;
    /*
     *  更新処理
     */
    void Update(class Engine& engine, float deltaTime) override;
    /*
     *  描画処理
     */
    void Render() override;
};
#endif // !_MAINGAMESCENE_H_



