/*
 *	@file	ActionDungeon.cpp
 *	@author	Seki
 */

#include "ActionDungeon.h"
#include "../../../Engine.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/Dungeon/DungeonDataLoader.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../Manager/StageManager.h"
#include "../../../Manager/CameraManager.h"
#include "../../../Manager/GameObjectManager.h"
#include "../../../Manager/CharacterManager.h"
#include "../../../Scene/Scene.h"
#include "../../../Component/ModelRenderer.h"
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
    if (!isComplete && !inputHandle && CheckHitKey(KEY_INPUT_2)) {
        inputHandle = true;
        isComplete = true;
        Teardown();
    }

    if (CheckHitKey(KEY_INPUT_0) == 0) inputHandle = false;
}
/*
 *	描画処理
 */
void ActionDungeon::Render() {
    StageManager::GetInstance().Render();
    DrawFormatString(50, 50, GetColor(0, 0, 0), "2 : AdvanveDay");
}
/*
 *  破棄処理
 */
void ActionDungeon::Teardown() {
    CharacterManager::GetInstance().RemoveCharacter(0);
    StageManager::GetInstance().Execute();

}

void ActionDungeon::DebugInitialize(Engine& engine, DungeonStageData& setStageData) {
    isComplete = false;
    stageData = setStageData;
    LoadManager& load = LoadManager::GetInstance();
    DungeonResource dungeonResource;

    GameObjectManager::GetInstance().Initialize(engine);
    CharacterManager::GetInstance().Initialize(engine);
    CameraManager::GetInstance().CreateCamera("camera", { 0, 0, 0 }, { 0, 0, 0 });
    CharacterManager::GetInstance().GeneratePlayer("player", { 0, 100, 0 }, { 0, 0, 0 }, { -0.5f, -1.0f, -0.5f }, { 0.5f,  1.0f,  0.5f });
    StageManager::GetInstance().Initialize(engine);

    std::string dungeonPath;
    if(!stageData.TryGetByLeafName("StageData", dungeonPath)) return;
    std::string dungeonBonePath;
    if(!stageData.TryGetByLeafName("StageBoneData", dungeonBonePath)) return;
    std::string playerPath;
    if(!stageData.TryGetByLeafName("PlayerData", playerPath)) return;

    dungeonResource.stageResource.push_back(load.LoadResource<LoadModel>(dungeonPath));
    dungeonResource.stageBoneResource = load.LoadResource<LoadJSON>(dungeonBonePath);
    dungeonResource.playerResource = load.LoadResource<LoadModel>(playerPath);

    load.SetOnComplete([this, &engine, dungeonResource]() {
        DebugSetup(engine, dungeonResource); 
    });
}

void ActionDungeon::DebugSetup(Engine& engine, const DungeonResource& setResource) {
    std::shared_ptr<LoadModel> stageModel = setResource.stageResource[0];
    int modelHandle = stageModel->GetHandle();
    int playerHandle = setResource.playerResource->GetHandle();
    StageManager::GetInstance().LoadStage(modelHandle);
    StageManager::GetInstance().SetStageJSONData(setResource.stageBoneResource->GetData());
    auto player = CharacterManager::GetInstance().GetCharacter(0)->GetOwner();
    player->AddComponent<ModelRenderer>();
    player->GetComponent<ModelRenderer>()->SetModel(playerHandle);
    player->position = StageManager::GetInstance().GetStartPos();
}
