/*
 *	@file	DebugScene.cpp
 *	@author	Seki
 */

#include "DebugScene.h"
#include "../Manager/CameraManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/CharacterManager.h"
#include "../Manager/StageManager.h"
#include "../Load/LoadManager.h"
#include "../Load/Model/LoadModel.h"
#include "../Component/ModelRenderer.h"

 /*
  *	初期化処理
  */
void DebugScene::Initialize(Engine& engine) {
	SetMouseDispFlag(false);
	GameObjectManager::GetInstance().Initialize(engine);
	CharacterManager::GetInstance().Initialize(engine);
	StageManager::GetInstance().Initialize(engine);
	auto stageModel = std::make_shared<LoadModel>("Res/Model/Stage/StageModel_1.mv1");
	auto playerModel = std::make_shared<LoadModel>("Res/Model/Player/RIPO_Model.mv1");
	auto enemyModel = std::make_shared<LoadModel>("Res/Model/Enemy/TutorialEnemy/EnemyModel.mv1");
	auto stageBoneData = std::make_shared<LoadJSON>("Data/Dungeon/Tutorial/TutorialDungeonCreatePos.json");
	CameraManager::GetInstance().CreateCamera("camera", { 0, 0, 0 }, { 0, 0, 0 });
	CharacterManager::GetInstance().GeneratePlayer("player", { 0, 0, 0 }, {0, 0, 0}, {-0.5f, -1.0f, -0.5f}, {0.5f,  1.0f,  0.5f});
	auto player = CharacterManager::GetInstance().GetCharacter(0);
	player->GetOwner()->AddComponent<ModelRenderer>();
	CharacterManager::GetInstance().GenerateEnemy("enemy", { 1, 0, 0 }, { 0, 0, 0 }, { -0.5f, -1.0f, -0.5f }, { 0.5f, 1.0f, 0.5f });
	auto enemy = CharacterManager::GetInstance().GetCharacter(1);
	enemy->GetOwner()->AddComponent<ModelRenderer>();

	LoadManager::GetInstance().AddLoader(stageModel);
	LoadManager::GetInstance().AddLoader(playerModel);
	LoadManager::GetInstance().AddLoader(enemyModel);
	LoadManager::GetInstance().AddLoader(stageBoneData);
	LoadManager::GetInstance().SetOnComplete(
		[stageModel, player, playerModel, enemy, enemyModel, stageBoneData]() {
			StageManager::GetInstance().LoadStage(stageModel->GetHandle());
			StageManager::GetInstance().SetStageJSONData(stageBoneData->GetData());
			int modelHandle = playerModel->GetHandle();
			player->GetOwner()->GetComponent<ModelRenderer>()->SetModel(modelHandle);
			player->GetOwner()->position = StageManager::GetInstance().GetStartPos();
			int enemyModelHandle = enemyModel->GetHandle();
			enemy->GetOwner()->GetComponent<ModelRenderer>()->SetModel(enemyModelHandle);
			
			//enemy->GetOwner()->position = StageManager::GetInstance().GetStartPos();
		}
	);
}
/*
 *	更新処理
 */
void DebugScene::Update(Engine& engine, float deltaTime) {
	Scene::Update(engine, deltaTime);
}
/*
 *	描画処理
 */
void DebugScene::Render() {
	{
		VECTOR pos1, pos2;

		//  XZ平面 100.0f毎に1本ライン引き
		{
			pos1 = VGet(-5000.0f, 0.0f, -5000.0f);
			pos2 = VGet(-5000.0f, 0.0f, 5000.0f);
			for (int i = 0; i <= 100; i++) {
				DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
				pos1.x += 100.0f;
				pos2.x += 100.0f;
			}

			pos1 = VGet(-5000.0f, 0.0f, -5000.0f);
			pos2 = VGet(5000.0f, 0.0f, -5000.0f);
			for (int i = 0; i <= 100; i++) {
				DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
				pos1.z += 100.0f;
				pos2.z += 100.0f;
			}
		}

		//  X軸
		{
			pos1 = VGet(0, 0, 0);
			pos2 = VScale(VGet(1, 0, 0), 5000.0f);		//  VGet(5000.0f, 0.0f, 0.0f)
			DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
		}
		//  Y軸
		{
			pos1 = VGet(0, 0, 0);
			pos2 = VScale(VGet(0, 1, 0), 5000.0f);		//  VGet(0.0f, 5000.0f, 0.0f)
			DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
		}
		//  Z軸
		{
			pos1 = VGet(0, 0, 0);
			pos2 = VScale(VGet(0, 0, 1), 5000.0f);	//  VGet(0.0f, 0.0f, 5000.0f)
			DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
		}

	}
	Scene::Render();
	StageManager::GetInstance().Render();

}
