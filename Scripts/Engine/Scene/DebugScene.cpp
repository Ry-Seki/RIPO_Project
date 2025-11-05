/*
 *	@file	DebugScene.cpp
 *	@author	Seki
 */

#include "DebugScene.h"
#include "../Manager/CameraManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/CharacterManager.h"
#include "../Manager/StageManager.h"
#include "../Manager/StageObjectManager.h"
#include "../Load/LoadManager.h"
#include "../Load/Model/LoadModel.h"
#include "../Component/ModelRenderer.h"

 /*
  *	初期化処理
  */
void DebugScene::Initialize(Engine& engine) {
	LoadManager& load = LoadManager::GetInstance();
	SetMouseDispFlag(false);
	GameObjectManager::GetInstance().Initialize(engine);
	CameraManager::GetInstance().Initialize(engine);
	CharacterManager::GetInstance().Initialize(engine);
	StageManager::GetInstance().Initialize(engine);
	StageObjectManager::GetInstance().Initialize(engine);
	auto stageModel = load.LoadResource<LoadModel>("Res/Model/Stage/StageModel_1.mv1");
	auto playerModel = load.LoadResource<LoadModel>("Res/Model/Player/RIPO_Model.mv1");
	auto enemyModel = load.LoadResource<LoadModel>("Res/Model/Enemy/TutorialEnemy/EnemyModel.mv1");
	auto stageBoneData = load.LoadResource<LoadJSON>("Data/Dungeon/Tutorial/TutorialDungeonCreatePos.json");
	auto treasureModel1 = load.LoadResource<LoadModel>("Res/Model/Treasure/Treasure01.mv1");
	auto treasureModel2 = load.LoadResource<LoadModel>("Res/Model/Treasure/Treasure02.mv1");
	CameraManager::GetInstance().CreateCamera("camera", { 0, 0, 0 }, { 0, 0, 0 });
	CharacterManager::GetInstance().GeneratePlayer("player", { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100,  300,  100 });
	auto player = CharacterManager::GetInstance().GetCharacter(0);
	CharacterManager::GetInstance().GenerateEnemy("enemy", { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 });
	CharacterManager::GetInstance().GenerateEnemy("enemy", { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 });
	CharacterManager::GetInstance().GenerateEnemy("enemy", { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 });
	std::vector<CharacterBasePtr> enemy(3);
	for (int i = 0; i < 3; i++) {
		enemy[i] = CharacterManager::GetInstance().GetCharacter(i + 1);
	}
	StageObjectManager::GetInstance().GenerateTreasure("treasure", { 0,0,0 }, { 0,0,0 }, { -100,0,-100 }, { 100,300,100 });
	StageObjectManager::GetInstance().GenerateTreasure("treasure", { 0,0,0 }, { 0,0,0 }, { -100,0,-100 }, { 100,300,100 });
	std::vector<StageObjectBasePtr> treasure(2);
	for (int i = 0; i < 2; i++) {
		treasure[i] = StageObjectManager::GetInstance().GetStageObject(i);
	}

	load.SetOnComplete(
		[stageModel, player, playerModel, enemy, enemyModel, stageBoneData, treasure, treasureModel1, treasureModel2]() {
			StageManager::GetInstance().LoadStage(stageModel->GetHandle());
			StageManager::GetInstance().SetStageJSONData(stageBoneData->GetData());
			int modelHandle = playerModel->GetHandle();
			CharacterManager::GetInstance().SetModelHandle(player->GetOwner(), modelHandle);
			player->GetOwner()->position = StageManager::GetInstance().GetStartPos();

			std::vector<Vector3> enemySpawnPos = StageManager::GetInstance().GetEnemySpwanPos();
			int enemyModelHandle = enemyModel->GetHandle();
			size_t enemySpawnCount = enemySpawnPos.size();
			for (int i = 0; i < enemy.size(); i++) {
				if (!enemy[i]) continue;
				CharacterManager::GetInstance().SetModelHandle(enemy[i]->GetOwner(), enemyModelHandle);
				std::shared_ptr<EnemyComponent> component = enemy[i]->GetOwner()->GetComponent<EnemyComponent>();
				if (!component) continue;
				enemy[i]->GetOwner()->position = enemySpawnPos[i];
				component->SetWayPoint(enemy[i]->GetOwner()->position);
				enemy[i]->GetOwner()->scale = { 4.5f,4.5f,4.5f };
			}

			std::vector<Vector3> treasureSpawnPos = StageManager::GetInstance().GetTreasureSpwanPos();
			std::vector<std::shared_ptr<LoadModel>> treasureModels = { treasureModel1, treasureModel2 };
			size_t treasureSpawnCount = treasureSpawnPos.size();

			for (int i = 0; i < treasureSpawnCount; i++) {
				int modelIndex = i % treasureModels.size();
				int treasureModelHandle = treasureModels[modelIndex]->GetHandle();

				StageObjectManager::GetInstance().SetModelHandle(treasure[i]->GetOwner(), treasureModelHandle);
				treasure[i]->GetOwner()->position = treasureSpawnPos[i];
			}
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
#if _DEBUG
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
	// 全オブジェクトのAABBCollider描画
	for (auto& obj : gameObjects) {
		auto aabb = obj->GetComponent<AABBCollider>();
		if (aabb == nullptr) continue;
		aabb->DebugRender();
	}

#endif
	Scene::Render();
	StageManager::GetInstance().Render();
#if _DEBUG
	// プレイヤーの位置表示
	GameObjectPtr player = CameraManager::GetInstance().GetTarget();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "PlayerPosition(%f,%f,%f)",
		player->position.x, player->position.y, player->position.z);
#endif
}
