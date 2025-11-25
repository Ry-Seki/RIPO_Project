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
#include "../../../Manager/StageObjectManager.h"
#include "../../../Manager/BulletManager.h"
#include "../../../Scene/Scene.h"
#include "../../../Component/ModelRenderer.h"
#include "../../../Component/Character/CharacterUtility.h"
#include "../../../Stage/StageObject/StageObjectUtility.h"
#include "../../../Audio/AudioUtility.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Stage/StageUtility.h"
#include "../../../GameObject/GameObjectUtility.h"
#include "../../../System/Money/MoneyManager.h"

#include <iostream>

 // 静的メンバ変数の初期化
bool ActionDungeon::isFirst = true;

// 名前空間の宣言
using namespace AudioUtility;
using namespace CharacterUtility;
using namespace StageUtility;
using namespace StageObjectUtility;
using namespace GameObjectUtility;

/*
 *	初期化処理
 */
void ActionDungeon::Initialize(Engine& engine) {

}
/*
 *  ロード済みのデータをセット(コールバック)
 */
void ActionDungeon::SetupData(Engine& engine) {

}
/*
 *	更新処理
 */
void ActionDungeon::Update(Engine& engine, float deltaTime) {
	if (isComplete) return;
	// 階段、ゴールフラグの取得
	bool exitFrag = GetExitFlag();
	bool stairFrag = GetStairMove();
	// 出口に触れたとき
	if (exitFrag) {
		// SEの再生
		PlaySE("GoalSE");
		inputHandle = true;
		isComplete = true;
		EndDungeon();
	}
	else if (stairFrag) {
		StageManager::GetInstance().NextStage();
		//StageManager::GetInstance().GetPrevStageHandle();
		//StageObjectManager::GetInstance().GenerateStair("stair", { 0,0,0 }, { 0,0,0 }, { -500,-500,-10 }, { 500,800,10 });
		//auto stair = StageObjectManager::GetInstance().GetStageObject(2);
		//Vector3 stairSpawnPos = StageManager::GetInstance().GetStairsPos();
		//stair->GetOwner()->position = stairSpawnPos;
	}
	if (!inputHandle && CheckHitKey(KEY_INPUT_2)) {
		// SEの再生
		PlaySE("DebugSE");
		inputHandle = true;
		isComplete = true;
		EndDungeon();
	}

	if (CheckHitKey(KEY_INPUT_0) == 0) inputHandle = false;
}
/*
 *	描画処理
 */
void ActionDungeon::Render() {
	StageManager::GetInstance().Render();
#if _DEBUG
	DrawFormatString(50, 50, GetColor(0, 0, 0), "2 : AdvanveDay");
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
#endif

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
/*
 *  破棄処理
 */
void ActionDungeon::Teardown() {

}

void ActionDungeon::DebugInitialize(Engine& engine, DungeonStageData& setStageData) {
	isComplete = false;
	stageData = setStageData;
	LoadManager& load = LoadManager::GetInstance();
	DungeonResource dungeonResource;
	auto treasureMap = stageData.GetCategory("Treasure");
	std::string leafKey;
	std::vector<int> IDList;
	for (const auto& [key, path] : treasureMap) {
		// 最後の階層だけを取得
		if (stageData.TryGetLeafKey(key, leafKey)) {
			IDList.push_back(std::stoi(leafKey));
		}
	}

	if (isFirst) {
		isFirst = false;
		GameObjectManager::GetInstance().Initialize(engine);
		CameraManager::GetInstance().Initialize(engine);
		CharacterManager::GetInstance().Initialize(engine);
		StageManager::GetInstance().Initialize(engine);
		StageObjectManager::GetInstance().Initialize(engine);
		BulletManager::GetInstance().Initialize(engine);
	}
	GeneratePlayer(GameConst::_CREATE_POSNAME_PLAYER, V_ZERO, V_ZERO, { -100, 0, -100 }, { 100,  300,  100 }, { 0, 100, 0 }, { 0,  200,  0 }, 200);
	CameraManager::GetInstance().CreateCamera("camera", { 0, 0, 0 }, { 0, 0, 0 });
	GenerateEnemy(GameConst::_CREATE_POSNAME_ENEMY, { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 }, { 0, 100, 0 }, { 0,  200,  0 }, 200);
	GenerateEnemy(GameConst::_CREATE_POSNAME_ENEMY, { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 }, { 0, 100, 0 }, { 0,  200,  0 }, 200);
	GenerateEnemy(GameConst::_CREATE_POSNAME_ENEMY, { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 }, { 0, 100, 0 }, { 0,  200,  0 }, 200);
	for (int i = 0; i < IDList.size(); i++) {
		GenerateTreasure("treasure", { 0,0,0 }, { 0,0,0 }, { -100,0,-100 }, { 100,300,100 }, IDList[i]);
	}
	GenerateStair("stair", { 0,0,0 }, { 0,0,0 }, { -500,-500,-10 }, { 500,800,10 });
	GenerateExit("exit", { 0,0,0 }, { 0,0,0 }, { -1000,-700,-10 }, { 1000,700,10 });

	LoadResourcesFromStageData(engine, stageData, dungeonResource);
}

void ActionDungeon::DebugSetup(Engine& engine, const DungeonResource& setResource) {
	// ステージの設定
	// モデルハンドルの取得
	int stageHandleCount = setResource.stageResource.size();
	std::vector<int> stageHandleList(stageHandleCount);
	for (int i = 0; i < stageHandleCount; i++) {
		stageHandleList[i] = setResource.stageResource[i]->GetHandle();
		LoadStage(stageHandleList);
	}
	// ステージボーンデータの設定
	SetStageJSONData(setResource.stageBoneResource[0]->GetData());

	// プレイヤーの設定
	// モデルの取得
	int playerHandle = setResource.playerResource->GetHandle();
	// プレイヤーオブジェクトの取得
	auto player = GetUseObject(0);
	if (!player) return;
	// 位置の設定
	player->position = GetStartPos();
	// モデルの設定
	SetModelHandle(player.get(), playerHandle);

	// 敵の設定
	// 敵の生成位置の取得
	std::vector<Vector3> enemySpawnPos = GetEnemySpwanPos();
	size_t enemyCount = enemySpawnPos.size();
	// モデルハンドルの取得
	int enemyHandle = setResource.enemyResource[0]->GetHandle();
	// 敵の取得
	std::vector<GameObjectPtr> enemyList = GetObjectByName(GameConst::_CREATE_POSNAME_ENEMY);
	for (int i = 0; i < enemyCount; i++) {
		if (enemyList.size() <= 0) break;
		auto enemyCharacter = enemyList[i];
		if (!enemyCharacter) continue;
		// 位置の設定
		enemyCharacter->position = enemySpawnPos[i];
		enemyCharacter->scale = { 4.5f, 4.5f, 4.5f };
		// モデルの設定
		SetModelHandle(enemyCharacter.get(), enemyHandle);
		// コンポーネントの取得
		std::shared_ptr<EnemyComponent> component = enemyCharacter->GetComponent<EnemyComponent>();
		if (!component) continue;
		// WayPointの設定
		component->SetWayPoint(enemyCharacter->position);
	}

	// 宝の設定
	// お宝の生成位置の取得
	std::vector<Vector3> treasureSpawnPos = GetTreasureSpwanPos();
	// 生成位置の要素数の取得
	size_t treasureCount = treasureSpawnPos.size();
	// ハンドルの要素数の取得
	size_t treasureTypeCount = setResource.treasureResource.size();
	for (int i = 0; i < treasureCount; i++) {
		// モデルハンドルの取得
		int treasureHandle = setResource.treasureResource[i]->GetHandle();
		// 宝の取得
		StageObjectBasePtr treasureObject = GetStageObject(i);
		if (!treasureObject) continue;
		// 宝オブジェクトの取得
		GameObject* treasure = GetStageObjectOwner(treasureObject);
		if (!treasure) continue;
		// 位置の設定
		treasure->position = treasureSpawnPos[i];
		// モデルの設定
		SetModelHandle(treasure, treasureHandle);
	}
	// 階段の設定
	int stairCount = treasureCount;
	auto stair = GetStageObject(stairCount);
	if (!stair) return;
	Vector3 stairSpawnPos = GetStairsPos();
	stair->GetOwner()->position = stairSpawnPos;

	// 出口の設定
	int exitCount = stairCount + 1;
	auto exit = GetStageObject(exitCount);
	if (!exit) return;
	Vector3 exitSpawnPos = GetGoalPos();
	exit->GetOwner()->position = exitSpawnPos;

	// フェードイン
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::NonStop);
	FadeManager::GetInstance().StartFade(fade);
	// 当たり判定の有効化
	SetUseObjectColliderFlag(true);
}

void ActionDungeon::EndDungeon() {
	// 当たり判定の無効化
	SetUseObjectColliderFlag(false);
	RemoveAllCharacter();
	StageManager::GetInstance().Execute();
	RemoveAllStageObject();
	CameraManager::GetInstance().ResetCamera();
}
/*
 *	ステージデータからロードリストに追加
 *	@param[in]	DungeonStageData& stageData			ステージデータ
 *	@param[in]	DungeonResource&  dungeonResource	セットするリソース
 */
void ActionDungeon::LoadResourcesFromStageData(Engine& engine, DungeonStageData& stageData, DungeonResource& dungeonResource) {
	LoadManager& load = LoadManager::GetInstance();
	// Stageカテゴリ
	auto stageMap = stageData.GetCategory("Stage");
	for (const auto& [key, path] : stageMap) {
		if (key.rfind("StageData", 0) == 0) {
			dungeonResource.stageResource.push_back(load.LoadResource<LoadModel>(path));
		}
		else if (key.rfind("StageBoneData", 0) == 0) {
			dungeonResource.stageBoneResource.push_back(load.LoadResource<LoadJSON>(path));
		}
	}
	// Characterカテゴリ
	auto characterMap = stageData.GetCategory("Character");
	for (const auto& [key, path] : characterMap) {
		if (key.find("Player") != std::string::npos) {
			dungeonResource.playerResource = load.LoadResource<LoadModel>(path);
		}
		else if (key.rfind("EnemyData") != std::string::npos) {
			dungeonResource.enemyResource.push_back(load.LoadResource<LoadModel>(path));
		}
	}
	// TreasureData
	auto treasureMap = stageData.GetCategory("Treasure");
	for (const auto& [key, path] : treasureMap) {
		// 数字キーだけを対象にする
		if (!path.empty() && std::all_of(key.begin(), key.end(), ::isdigit)) {
			dungeonResource.treasureResource.push_back(load.LoadResource<LoadModel>(path));
		}
	}
	// EventTreasureData（後ほどフラグで管理）
	std::string eventTreasurePath;
	if (stageData.TryGet("Treasure.EventTreasureData", eventTreasurePath) && !eventTreasurePath.empty()) {
		dungeonResource.eventTreasureResource = load.LoadResource<LoadModel>(eventTreasurePath);
	}    // ロード完了時のコールバック登録
	load.SetOnComplete([this, &engine, dungeonResource]() {
		DebugSetup(engine, dungeonResource);
	});
}
