/*
 *  @file   MainGameScene.h
 */

#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_
#include "Scene.h"
#include <DxLib.h>

class MainGameScene : public Scene {
public:
    void Initialize(class Engine& engine) override;
    void Update(class Engine& engine, float deltaTime) override;
    void Render() override;
};
#endif // !_MAINGAMESCENE_H_



