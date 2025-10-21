/*
 *	@file	ActionDungeon.cpp
 *	@author	Seki
 */

#include "ActionDungeon.h"
#include "../../../Engine.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/Dungeon/DungeonDataLoader.h"
#include "../../../Manager/StageManager.h"
#include "../../../Manager/CameraManager.h"
#include "../../../Manager/GameObjectManager.h"
#include "../../../Manager/CharacterManager.h"
#include "../../../Scene/Scene.h"

#include <iostream>

/*
 *	初期化処理
 */
void ActionDungeon::Initialize(Engine& engine) {

}
/*
 *  ロード済みのデータをセット(コールバック)
 */
void ActionDungeon::Setup(Engine& engine) {

}
/*
 *	更新処理
 */
void ActionDungeon::Update(Engine& engine, float deltaTime) {
}
/*
 *	描画処理
 */
void ActionDungeon::Render() {
    StageManager::GetInstance().Render();
}

void ActionDungeon::DebugInitialize(Engine& engine, std::string setFilePath) {
    auto dungeonModel = std::make_shared<LoadModel>(setFilePath);
    LoadManager::GetInstance().AddLoader(dungeonModel);
    LoadManager::GetInstance().SetOnComplete([this, &engine, dungeonModel]() {DebugSetup(engine, dungeonModel); });
}

void ActionDungeon::DebugSetup(Engine& engine, std::shared_ptr<LoadModel> setModel) {
    int modelHandle = setModel->GetHandle();
    GameObjectManager::GetInstance().Initialize(engine);
    CharacterManager::GetInstance().Initialize(engine);
    CameraManager::GetInstance().CreateCamera("camera", { 0, 0, 0 }, { 0, 0, 0 });
    CharacterManager::GetInstance().GeneratePlayer("player", { 0, 100, 0 }, { 0, 0, 0 }, { -0.5f, -1.0f, -0.5f }, { 0.5f,  1.0f,  0.5f });
    StageManager::GetInstance().Initialize(engine);
    StageManager::GetInstance().LoadStage(modelHandle);
    LoadManager::GetInstance().Clear();
}
