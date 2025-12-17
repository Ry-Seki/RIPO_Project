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
#include "../../../Component/GravityComponent.h"
#include "../../../GameObject/GameObjectUtility.h"
#include "../../../System/Money/MoneyManager.h"
#include "../../../Stage/StageObject/Treasure/Treasure.h"

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
	if (!isStart || isComplete) return;

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
		ChangeFloor();
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
	if (!isStart) return;

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
	delete holdTreasure;
}
/*
 *	@brief		階層変更
 */
void ActionDungeon::ChangeFloor() {
	// 当たり判定の解除
	SetUseObjectColliderFlag(false);
	// 現在の階層の片付け処理
	TeardownCurrentFloor();
	// 階層の変更
	StageManager::GetInstance().NextStage(nextFloor);
	currentFloor = nextFloor;
	// 次の階層の準備処理
	SetupNextFloor();
}
/*
 *	@brief		現在の階層の片付け処理
 */
void ActionDungeon::TeardownCurrentFloor() {
	// 当たり判定の解除
	SetUseObjectColliderFlag(false);
	StageObjectManager::GetInstance().ClearObject();
	// 現在のフロアの敵のデータを設定
	ConvertEnemyData();
	// 現在残っている敵の片付け処理
	TeardownEnemy();
	// プレイヤーが持っているお宝以外を削除
	TeardownStageObject();
}
/*
 *	@brief		次の階層の準備
 */
void ActionDungeon::SetupNextFloor() {
	auto treasureMap = stageData.GetCategory("Treasure");
	std::string leafKey;
	std::vector<int> IDList;
	for (const auto& [key, path] : treasureMap) {
		// 最後の階層だけを取得
		if (stageData.TryGetLeafKey(key, leafKey)) {
			IDList.push_back(std::stoi(leafKey));
		}
	}
	// プレイヤーが持っているお宝IDの取得
	int holdTreasureID = -1;
	if (holdTreasure) {
		auto component = holdTreasure->GetComponent<StageObjectBase>();
		holdTreasureID = component->GetTreasureID();
	}
	// ダンジョンの再構築
	FloorData setFloorData;
	// フロアデータの取得
	floorData.TryGetFloorData(currentFloor, setFloorData);
	// 初めてかどうかで分ける
	if (setFloorData.isFirst) {
		setFloorData.isFirst = false;
	}
	else {
		if (enemyFloorList[currentFloor].size() > 0) {
			setFloorData.enemySpawnCount = enemyFloorList[currentFloor].size();
		}
	}
	dungeonCreater.SetFloorData(setFloorData);
	dungeonCreater.RegenerateDungeon(currentFloor, enemyFloorList[currentFloor], holdTreasureID, IDList, nextFloor);
	// フロアデータの更新
	floorData.TrySetFloorData(currentFloor, setFloorData);
	// フェードイン
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::NonStop);
	FadeManager::GetInstance().StartFade(fade);
	// 当たり判定の有効化
	SetUseObjectColliderFlag(true);
}
/*
 *	@brief		敵の再生成に必要なデータへ変換
 */
void ActionDungeon::ConvertEnemyData() {
	// 要素数を空にする
	enemyFloorList[currentFloor].clear();
	auto enemyList = GetObjectByName(GameConst::_CREATE_POSNAME_ENEMY);
	for (int i = 0, max = enemyList.size(); i < max; i++) {
		auto enemy = enemyList[i];
		if (!enemy) continue;

		auto component = enemy->GetComponent<EnemyComponent>();
		if (!component) continue;

		enemyFloorList[currentFloor].push_back(component->GetSpawnEnemyID());
	}
}
/*
 *	@brief		敵の片付け処理
 */
void ActionDungeon::TeardownEnemy() {
	GameObjectList deleteEnemyList = GetObjectByName(GameConst::_CREATE_POSNAME_ENEMY);
	for (auto& enemy : deleteEnemyList) {
		if (!enemy) continue;
		int deleteID = enemy->ID;
		RemoveCharacter(deleteID);
	}
}
/*
 *	@brief		ステージオブジェクトの片付け処理
 */
void ActionDungeon::TeardownStageObject() {
	holdTreasure = StageManager::GetInstance().GetLiftObject();
	GameObjectList stageObjectList = GetCreateObjectList();
	// プレイヤーが所持しているお宝以外を削除する
	for (int i = stageObjectList.size() - 1; i >= 0; i--) {
		GameObject* stageObject = stageObjectList[i].get();
		if (!stageObject) continue;

		int ID = stageObject->ID;
		if (holdTreasure) {
			if (ID == holdTreasure->ID) continue;

			RemoveStageObject(ID);
		}
		else {
			RemoveStageObject(ID);
		}
	}
}
void ActionDungeon::DebugInitialize(Engine& engine, DungeonStageData& setStageData, DungeonFloorData& setFloorData) {
	isComplete = false;
	currentFloor = 0;
	enemyFloorList.resize(16);
	stageData = setStageData;
	floorData = setFloorData;
	LoadManager& load = LoadManager::GetInstance();
	//DungeonResource dungeonResource;
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
	// リソースの読み込み
	resourceData.LoadResourcesFromStageData(stageData);
	// ロード完了時のコールバック登録
	load.SetOnComplete([this, &engine, IDList]() {
		isStart = true;
		// ダンジョンの生成
		FloorData setFloorData;
		floorData.TryGetFloorData(currentFloor, setFloorData);
		setFloorData.isFirst = false;
		dungeonCreater.SetDungeonData(setFloorData, resourceData);
		dungeonCreater.GenerateDungeon(currentFloor, IDList, nextFloor);
		floorData.TrySetFloorData(currentFloor, setFloorData);
		// 重力
		auto player = CharacterManager::GetInstance().GetPlayer();
		SetUseObjectColliderFlag(true);
		player->GetComponent<GravityComponent>()->SetGravity(true);
		});
}

void ActionDungeon::DebugSetupData(Engine& engine, const DungeonResourceData& setResource) {
	currentFloor = 0;
	enemyFloorList.resize(16);
	enemyFloorList[currentFloor].resize(16);


	// ステージの設定
	// モデルハンドルの取得
	int stageHandleCount = setResource.stageResource.size();
	std::vector<int> stageHandleList(stageHandleCount);
	for (int i = 0; i < stageHandleCount; i++) {
		stageHandleList[i] = setResource.stageResource[i]->GetHandle();
	}
	LoadStage(stageHandleList);
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
	std::vector<int> enemySpawnIDList;
	std::vector<Vector3> enemySpawnPos = GetEnemySpwanPos(enemySpawnIDList);
	size_t enemyCount = enemySpawnPos.size();
	// モデルハンドルの取得
	int enemyHandle = setResource.enemyResource[0]->GetHandle();
	// 敵の取得

	// 宝の設定
	// お宝の生成位置の取得
	std::vector<Vector3> treasureSpawnPos = GetTreasureSpwanPos();
	// 生成位置の要素数の取得
	size_t treasureCount = treasureSpawnPos.size();
	// ハンドルの要素数の取得
	size_t treasureTypeCount = setResource.treasureResource.size();
	GameObjectList treasureList = GetObjectByName(GameConst::_CREATE_POSNAME_TREASURE);
	// お宝の取得
	for (int i = 0; i < treasureCount; i++) {
		// モデルハンドルの取得
		int treasureHandle = setResource.treasureResource[i]->GetHandle();
		// 宝オブジェクトの取得
		GameObject* treasure = treasureList[i].get();
		if (!treasure) continue;
		// 位置の設定
		treasure->position = treasureSpawnPos[i];
		// モデルの設定
		SetModelHandle(treasure, treasureHandle);
	}
	// 階段の設定
	auto stair = GetStageObject("Stair");
	if (!stair) return;
	std::vector<Vector3> stairSpawnPos = GetStairsPos();
	size_t stairSpawnPosCount = stairSpawnPos.size();
	for (int i = 0; i < stairSpawnPosCount; i++) {
		stair->position = stairSpawnPos[i];
	}

	// 出口の設定
	auto exit = GetStageObject("Exit");
	if (!exit) return;
	std::vector<Vector3> exitSpawnPos = GetGoalPos();
	size_t exitPointCount = exitSpawnPos.size();
	for (int i = 0; i < exitPointCount; i++) {
		exit->position = exitSpawnPos[i];

	}

	// フェードイン
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::NonStop);
	FadeManager::GetInstance().StartFade(fade);
	// 当たり判定の有効化
	SetUseObjectColliderFlag(true);
}

void ActionDungeon::EndDungeon() {
	holdTreasure = nullptr;
	// 当たり判定の無効化
	SetUseObjectColliderFlag(false);
	RemoveAllCharacter();
	StageManager::GetInstance().Execute();
	RemoveAllStageObject();
	CameraManager::GetInstance().ResetCamera();
}