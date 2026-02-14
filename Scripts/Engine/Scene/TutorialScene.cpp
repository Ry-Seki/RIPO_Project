/*
 *	@file	TutorialScene.cpp
 *	@author	Seki
 */

#include "TutorialScene.h"
#include "ResultScene.h"
#include "MainGameScene.h"
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
#include "../Manager/WeaponManager.h"
#include "../../Data/WeaponDataManager.h"
#include "../Component/Character/HPBarComponent.h"
#include "../UI/PlayerUI/PlayerUI.h"
#include "../Scripts/Engine/Manager/EnemyDataManager.h"
#include "../Menu/Dungeon/DungeonTreasureUI.h"

 /*
  *	コンストラクタ
  */
TutorialScene::TutorialScene() {
}
/*
 *	デストラクタ
 */
TutorialScene::~TutorialScene() {
}
/*
 *	初期化処理
 */
void TutorialScene::Initialize(Engine& engine) {
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
	WeaponManager::GetInstance().Initialize();
	WeaponDataManager::GetInstance().Initialize();
	EnemyDataManager::GetInstance().Initialize();

}
/*
 *	ロード済みデータのセット (コールバック)
 */
void TutorialScene::SetupData(Engine& engine) {
}
/*
 *	@brief	準備前処理
 */
void TutorialScene::Setup(Engine& engine) {
	auto& save = SaveDataManager::GetInstance();
	auto& context = gameState->GetActionContext();
	save.ApplyLoadData(context);
	gameState->ChageState(GameEnum::GameState::InitialTutorial);
}
/*
 *	更新処理
 */
void TutorialScene::Update(Engine& engine, float deltaTime) {
	gameState->Update(deltaTime);
	Scene::Update(engine, deltaTime);
	// 遷移条件
	if (gameState->IsActionEnd()) {
		EndTutorialScene(engine);
	}
}
/*
 *	描画処理
 */
void TutorialScene::Render() {
	gameState->Render();
	Scene::Render();
	EffectManager::GetInstance().Render();
	DrawFormatString(50, 400, GetColor(255, 255, 255), "Money : %d", MoneyManager::GetInstance().GetCurrentMoney());

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
}

/*
 *  @brief  メインシーン終了処理
 */
void TutorialScene::EndTutorialScene(Engine& engine) {
	auto& context = gameState->GetActionContext();
	auto& save = SaveDataManager::GetInstance();
	// アクション終了フラグの変更
	gameState->SetIsActionEnd(false);
	context.prevIncome = 0;
	MoneyManager::GetInstance().ResetMoney();
	// オートセーブ
	save.CollectSaveData(context);
	save.AutoSave();
	FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::NonStop);
	FadeManager::GetInstance().StartFade(fadeOut, [&engine, this]() {
		engine.SetNextScene(std::make_shared<MainGameScene>());
	});
}
