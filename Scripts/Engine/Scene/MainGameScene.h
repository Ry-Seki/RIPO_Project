/*
 *  @file   MainGameScene.h
 */

#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_
#include "Scene.h"
#include <DxLib.h>

class MainGameScene : public Scene {
public:
    bool Initialize(class Engine& engine) override {
        DrawFormatString(50, 50, GetColor(0, 255, 0), "[MainGameScene] ŠJŽn");
        return true;
    }
    void Update(class Engine& engine, float deltaTime) override {}
};
#endif // !_MAINGAMESCENE_H_



