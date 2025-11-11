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
#include "../Manager/BulletManager.h"
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
	BulletManager::GetInstance().Initialize(engine);
	auto stageModel = load.LoadResource<LoadModel>("Res/Model/Stage/StageModel_1.mv1");
	auto stageModel2 = load.LoadResource<LoadModel>("Res/Model/Stage/StageModel_1_TwoLayer.mv1");
	auto playerModel = load.LoadResource<LoadModel>("Res/Model/Player/RIPO_Model.mv1");
	auto enemyModel = load.LoadResource<LoadModel>("Res/Model/Enemy/TutorialEnemy/EnemyModel.mv1");
	auto stageBoneData = load.LoadResource<LoadJSON>("Data/Dungeon/Tutorial/TutorialDungeonCreatePos.json");
	auto treasureModel1 = load.LoadResource<LoadModel>("Res/Model/Treasure/Treasure01.mv1");
	auto treasureModel2 = load.LoadResource<LoadModel>("Res/Model/Treasure/Treasure02.mv1");
	auto player = CharacterManager::GetInstance().GeneratePlayer("player", { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100,  300,  100 });
	CameraManager::GetInstance().CreateCamera("camera", { 0, 0, 0 }, { 0, 0, 0 });
	std::vector<GameObjectPtr> enemy(3);
	enemy[0] = CharacterManager::GetInstance().GenerateEnemy("enemy", { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 });
	enemy[1] = CharacterManager::GetInstance().GenerateEnemy("enemy", { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 });
	enemy[2] = CharacterManager::GetInstance().GenerateEnemy("enemy", { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 });
	StageObjectManager::GetInstance().GenerateTreasure("treasure", { 0,0,0 }, { 0,0,0 }, { -100,0,-100 }, { 100,300,100 });
	StageObjectManager::GetInstance().GenerateTreasure("treasure", { 0,0,0 }, { 0,0,0 }, { -100,0,-100 }, { 100,300,100 });
	std::vector<StageObjectBasePtr> treasure(2);
	for (int i = 0; i < 2; i++) {
		treasure[i] = StageObjectManager::GetInstance().GetStageObject(i);
	}
	StageObjectManager::GetInstance().GenerateStair("stair", { 0,0,0 }, { 0,0,0 }, { -500,-500,-10 }, { 500,800,10 });
	auto stair = StageObjectManager::GetInstance().GetStageObject(2);
	StageObjectManager::GetInstance().GenerateExit("exit", { 0,0,0 }, { 0,0,0 }, { -1000,-700,-10 }, { 1000,700,10 });
	auto exit = StageObjectManager::GetInstance().GetStageObject(3);

	load.SetOnComplete(
		[stageModel, stageModel2, player, playerModel, enemy, enemyModel, stageBoneData, treasure, treasureModel1, treasureModel2, stair, exit]() {

			std::vector<int> stageHandles = {
				stageModel->GetHandle(),
				stageModel2->GetHandle()
			};
			StageManager::GetInstance().LoadStage(stageHandles);
			StageManager::GetInstance().SetStageJSONData(stageBoneData->GetData());
			int modelHandle = playerModel->GetHandle();
			CharacterManager::GetInstance().SetModelHandle(player.get(), modelHandle);
			player->position = StageManager::GetInstance().GetStartPos();

			std::vector<Vector3> enemySpawnPos = StageManager::GetInstance().GetEnemySpwanPos();
			int enemyModelHandle = enemyModel->GetHandle();
			size_t enemySpawnCount = enemySpawnPos.size();
			for (int i = 0; i < enemy.size(); i++) {
				if (!enemy[i]) continue;
				CharacterManager::GetInstance().SetModelHandle(enemy[i].get(), enemyModelHandle);
				std::shared_ptr<EnemyComponent> component = enemy[i]->GetComponent<EnemyComponent>();
				if (!component) continue;
				enemy[i]->position = enemySpawnPos[i];
				component->SetWayPoint(enemy[i]->position);
				enemy[i]->scale = { 4.5f,4.5f,4.5f };
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

			Vector3 stairSpawnPos = StageManager::GetInstance().GetStairsPos();
			stair->GetOwner()->position = stairSpawnPos;

			Vector3 exitSpawnPos = StageManager::GetInstance().GetGoalPos();
			exit->GetOwner()->position = exitSpawnPos;
			GameObjectManager::GetInstance().SetObjectColliderFlag(true);
		}
	);
}
/*
 *	更新処理
 */
void DebugScene::Update(Engine& engine, float deltaTime) {
	Scene::Update(engine, deltaTime);

	bool stairFrag = StageObjectManager::GetInstance().GetStairMove();
	if (stairFrag) {
		StageManager::GetInstance().NextStage();


	}

}
/*
 *	描画処理
 */
void DebugScene::Render() {
#if _DEBUG
	//  XZ平面 100.0f毎に1本ライン引き
	{
		VECTOR pos1, pos2;

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
	// オブジェクトのTransform表示
	{
		// プレイヤーの位置表示
		GameObjectPtr player = CameraManager::GetInstance().GetTarget();
		DrawFormatString(0, 0, GetColor(255, 255, 255), "PlayerPosition(%f,%f,%f)",
			player->position.x, player->position.y, player->position.z);
		// カメラの角度表示
		GameObjectPtr camera = CameraManager::GetInstance().GetCamera();
		DrawFormatString(0, 20, GetColor(255, 255, 255), "CameraRotation(%f,%f,%f)",
			camera->rotation.x, camera->rotation.y, camera->rotation.z);
		bool stairFrag = StageObjectManager::GetInstance().GetStairMove();
		if (stairFrag) {
			DrawFormatString(0, 40, GetColor(255, 255, 255), "StairFrag_true");
		}
		else {
			DrawFormatString(0, 40, GetColor(255, 255, 255), "StairFrag_false");
		}

		bool exitFrag = StageObjectManager::GetInstance().GetExitFlag();
		if (exitFrag) {
			DrawFormatString(0, 60, GetColor(255, 255, 255), "ExitFrag_true");
		}
		else {
			DrawFormatString(0, 60, GetColor(255, 255, 255), "ExitFrag_false");
		}

	}
#endif
}
