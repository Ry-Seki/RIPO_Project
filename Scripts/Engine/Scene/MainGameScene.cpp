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
#include "../Scripts/Engine/Manager/EffectManager.h"
#include "../../Data/WeaponDataManager.h"
#include "../Component/Character/HPBarComponent.h"
#include "../UI/PlayerUI/ReticleUI.h"

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
	gameState->ChageState(GameEnum::GameState::SelectAction);
	MenuManager::GetInstance().GetMenu<MenuInGame>();
	MenuManager::GetInstance().GetMenu<ReticleUI>();
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
}
/*
 *  @brief  準備前処理
 */
void MainGameScene::Setup(Engine& engine) {
	auto& save = SaveDataManager::GetInstance();
	save.LoadCurrentSlot();

	auto& context = gameState->GetActionContext();
	save.ApplyLoadData(context);
}
/*
 *  更新処理
 */
void MainGameScene::Update(Engine& engine, float deltaTime) {
	gameState->Update(deltaTime);
	Scene::Update(engine, deltaTime);
	EffectManager::GetInstance().Update();
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
#endif
	// 敵の攻撃仮描画
	for (auto& obj : gameObjects) {
		if (obj->name != GameConst::_CREATE_POSNAME_ENEMY)
			continue;
		auto aabb = obj->GetComponent<AABBCollider>();
		if (aabb != nullptr)
			aabb->DebugRender();
	}

	// HPゲージ
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
void MainGameScene::EndMainGameScene(Engine& engine) {
	auto& context = gameState->GetActionContext();
	auto& save = SaveDataManager::GetInstance();
	// アクション終了フラグの変更
	gameState->SetIsActionEnd(false);
	// 現在選択されているスロットにセーブ
	save.CollectSaveData(context);
	save.SaveCurrentSlot();
	// 現在選択されているスロットがオートセーブでなければオートセーブする
	if (save.GetCurrentSlot() != 0) save.AutoSave();
	// シーン遷移条件
	if (context.elapsedDay > _END_DAY) {
		FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::NonStop);
		FadeManager::GetInstance().StartFade(fadeOut, [&engine, this]() {
			engine.SetNextScene(std::make_shared<ResultScene>());
			});
	}
	else {
		FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::NonStop);
		FadeManager::GetInstance().StartFade(fadeOut, [&engine, this]() {
			gameState->ChageState(GameEnum::GameState::SelectAction);
			});
	}
}
