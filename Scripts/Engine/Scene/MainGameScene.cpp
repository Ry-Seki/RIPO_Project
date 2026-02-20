/*
 *  @file   MainGameScene.cpp
 *  @author Seki
 */

#include "MainGameScene.h"
#include "ResultScene.h"
#include "../Engine.h"
#include "../Fade/FadeFactory.h"
#include "../Fade/FadeManager.h"
#include "../Manager/StageManager.h"
#include "../Load/JSON/LoadJSON.h"
#include "../System/Money/MoneyManager.h"
#include "../System/Status/PlayerStatusManager.h"
#include "../Component/CapsuleCollider.h"
#include "../Save/SaveDataManager.h"
#include "../Menu/MenuManager.h"
#include "../Menu/MainGame/MenuInGame.h"
#include "../Manager/EffectManager.h" 
#include "../../Data/WeaponDataManager.h"
#include "../Component/Character/HPBarComponent.h"
#include "../Component/Character/BossHPBarComponentr.h"
#include "../UI/PlayerUI/PlayerUI.h"
#include "../Scripts/Engine/Manager/EnemyDataManager.h"
#include "../Menu/Dungeon/DungeonTreasureUI.h"

 /*
  *  @brief  デストラクタ
  */
MainGameScene::~MainGameScene() = default;

/*
 *  初期化処理
 */
void MainGameScene::Initialize(Engine& engine) {
	// ゲームステートの初期化
	gameState = std::make_unique<GameStateMachine>();
	gameState->Initialize(engine);
	MenuManager::GetInstance().GetMenu<MenuInGame>();
	MenuManager::GetInstance().GetMenu<PlayerUI>();
	MenuManager::GetInstance().GetMenu<DungeonTreasureUI>();
	auto treasureData = LoadManager::GetInstance().LoadResource<LoadJSON>(_TREASURE_DATA_PATH);
	auto itemData = LoadManager::GetInstance().LoadResource<LoadJSON>(_ITEM_DATA_PATH);
	auto effectData = LoadManager::GetInstance().LoadResource<LoadJSON>(_EFFECT_DATA_PATH);
	LoadManager::GetInstance().SetOnComplete([this, &engine, treasureData, itemData, effectData]() {
		MoneyManager::GetInstance().LoadFromJSON(treasureData->GetData(), itemData->GetData());
		EffectManager::GetInstance().SetEffectJSONData(effectData->GetData());
		EffectManager::GetInstance().Initialize(engine);
		Setup(engine);
		});
	PlayerStatusManager::GetInstance().Initialize();
	WeaponDataManager::GetInstance().Initialize();
	EnemyDataManager::GetInstance().Initialize();

}
/*
 *  @brief  準備前処理
 */
void MainGameScene::Setup(Engine& engine) {
	auto& save = SaveDataManager::GetInstance();

	auto& context = gameState->GetActionContext();
	save.ApplyLoadData(context);
	gameState->ChageState(GameEnum::GameState::SelectAction);
}
/*
 *  更新処理
 */
void MainGameScene::Update(Engine& engine, float deltaTime) {
	gameState->Update(deltaTime);
	Scene::Update(engine, deltaTime);
	// 遷移条件
	if (gameState->IsActionEnd()) {
		EndMainGameScene(engine);
	}
}
/*
 *  描画処理
 */
void MainGameScene::Render() {
	gameState->Render();
	Scene::Render();
	EffectManager::GetInstance().Render();

#if _DEBUG
	// 全オブジェクトのCollider描画
	for (auto& obj : gameObjects) {
		auto aabb = obj->GetComponent<AABBCollider>();
		if (aabb != nullptr)
			aabb->DebugRender();
		auto capsule = obj->GetComponent<CapsuleCollider>();
		if (capsule != nullptr)
			capsule->DebugRender();
	}
	// 敵の攻撃仮描画
	for (auto& obj : gameObjects) {
		if (obj->name != GameConst::_CREATE_POSNAME_ENEMY)
			continue;
		auto aabb = obj->GetComponent<AABBCollider>();
		if (aabb != nullptr)
			aabb->DebugRender();
	}
#endif

	// エネミーHPゲージ
	for (auto& obj : gameObjects) {
		if (obj->name != GameConst::_CREATE_POSNAME_ENEMY)
			continue;
		auto HPBar = obj->GetComponent<HPBarComponent>();
		if (HPBar != nullptr)
			HPBar->ShowHPBar();
	}
	// ボスのHPゲージ
	for (auto& obj : gameObjects) {
		if (obj->name != GameConst::_CREATE_POSNAME_BOSS)
			continue;
		auto HPBar = obj->GetComponent<BossHPBarComponent>();
		if (HPBar != nullptr)
			HPBar->ShowHPBar();
	}
}
/*
 *  @brief  メインシーン終了処理
 */
void MainGameScene::EndMainGameScene(Engine& engine) {
	auto& context = gameState->GetActionContext();
	auto& save = SaveDataManager::GetInstance();
	// アクション終了フラグの変更
	gameState->SetIsActionEnd(false);
	context.prevIncome = 0;
	// シーン遷移条件
	if (context.elapsedDay > GameConst::END_DAY) {
		context.isClear = true;
		context.elapsedDay--;
		engine.SetNextScene(std::make_shared<ResultScene>());
	}
	else {
		gameState->ChageState(GameEnum::GameState::SelectAction);
	}
	save.CollectSaveData(context);
	save.AutoSave();
}
