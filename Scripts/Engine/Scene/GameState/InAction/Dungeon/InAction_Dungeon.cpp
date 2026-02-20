/*
 *	@file	InAction_Dungeon.cpp
 *	@author	Seki
 */

#include "InAction_Dungeon.h"
#include "../GameState_InAction.h"
#include "../../GameStateMachine.h"
#include "../../../../Manager/GameObjectManager.h"
#include "../../../../Manager/BulletManager.h"
#include "../../../../Manager/CameraManager.h"
#include "../../../../Manager/CharacterManager.h"
#include "../../../../Manager/StageManager.h"
#include "../../../../Manager/StageObjectManager.h"
#include "../../../../Menu/MenuManager.h"
#include "../../../../Load/LoadManager.h"
#include "../../../../Component/GravityComponent.h"
#include "../../../../Audio/AudioUtility.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../System/World/WorldProgressManager.h"
#include "../../../../Input/InputUtility.h"
#include "../../../../Manager/EffectManager.h"
#include "../../../../Load/Audio/LoadAudio.h"

 /*
  *	@brief	初期化処理
  */
void InAction_Dungeon::Initialize(Engine& engine) {
	GameObjectManager::GetInstance().Initialize(engine);
	CameraManager::GetInstance().Initialize(engine);
	CharacterManager::GetInstance().Initialize(engine);
	StageManager::GetInstance().Initialize(engine);
	StageObjectManager::GetInstance().Initialize(engine);
	BulletManager::GetInstance().Initialize(engine);
	auto dungeonBGM01 = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/BGM/Dungeon/Dungeon_1/星の詩.mp3");
	auto dungeonBGM02 = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/BGM/Dungeon/Dungeon_2/Apparition’s_Lullaby.mp3");
	LoadManager::GetInstance().SetOnComplete([&engine, this, dungeonBGM01, dungeonBGM02]() {
		AudioUtility::RegisterBGMHandle("dungeonBGM01", dungeonBGM01->GetHandle());
		AudioUtility::RegisterBGMHandle("dungeonBGM02", dungeonBGM02->GetHandle());
		});
}
/*
 *	@brief	準備前処理
 */
void InAction_Dungeon::Setup() {
	isStart = false;
	auto& context = owner->GetOwner()->GetActionContext();
	floorProcessor.CreateFloor(context, isStart, treasureIDList);
	SetMouseDispFlag(false);
}
/*
 *	@brief	更新処理
 */
void InAction_Dungeon::Update(float deltaTime) {
	if (!isStart) return;

	StageObjectManager& stageObject = StageObjectManager::GetInstance();
	// 階段、ゴールフラグの取得
	bool exitFrag = stageObject.GetExitFlag();
	bool stairFrag = stageObject.GetStairMove();
	// プレイヤーが死亡した場合
	if (IsPlayerDead()) {
		auto& context = owner->GetOwner()->GetActionContext();
		context.isPlayerDead = true;
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.5f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			EndDungeon();
			});
	}
	// 出口に触れたとき
	else if (exitFrag) {
		// SEの再生
		AudioUtility::PlaySE("GoalSE");
		FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.5f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fade, [this]() {
			EndDungeon();
			});
	}
	else if (stairFrag) {
		floorProcessor.ChangeFloor();
	}
	EffectManager::GetInstance().Update();
}
/*
 *	@brief	描画処理
 */
void InAction_Dungeon::Render() {
	if (!isStart) return;

	StageManager::GetInstance().Render();
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
	PlayerStatusData BaseStatus = PlayerStatusManager::GetInstance().GetPlayerStatusData();
	PlayerStatusValue playerStatus = GetPlayer()->GetComponent<PlayerComponent>()->GetPlayerStatus();
	DrawFormatString(450, 20, GetColor(255, 255, 255), "HP : %d / %d", playerStatus.HP, BaseStatus.base.HP);
	DrawFormatString(450, 40, GetColor(255, 255, 255), "Stamina : %d / %d", playerStatus.stamina, BaseStatus.base.stamina);
	DrawFormatString(450, 60, GetColor(255, 255, 255), "Strength : %d", BaseStatus.base.strength);
	DrawFormatString(450, 80, GetColor(255, 255, 255), "ResistTime : %d / %d", playerStatus.resistTime, BaseStatus.base.resistTime);
#endif
}
/*
 *	@brief	片付け処理
 */
void InAction_Dungeon::Teardown() {
	SetMouseDispFlag(true);
	MenuManager::GetInstance().CloseAllMenu();
}
/*
 *	@brief		プレイヤーの死亡判定
 *	@return		bool
 */
bool InAction_Dungeon::IsPlayerDead() {
	auto player = CharacterUtility::GetPlayer();
	if (!player) return false;

	auto component = player->GetComponent<PlayerComponent>();
	if (!component) return false;

	return component->GetIsDead();
}
/*
 *	@brief	ダンジョン終了処理
 */
void InAction_Dungeon::EndDungeon() {
	isStart = false;
	SetMouseDispFlag(TRUE);
	InputUtility::SetActionMapIsActive(GameEnum::ActionMap::PlayerAction, false);
	auto& context = owner->GetOwner()->GetActionContext();
	CalculationDungeon(context.dungeonID, context.isCurrentEvent);
	context.isPlayerDead = IsPlayerDead();
	context.isCurrentEvent = false;
	context.dungeonStageData.ClearDungeonStageMap();
	floorProcessor.EndDungeon();
	owner->GetOwner()->ChageState(GameEnum::GameState::ResultAction);
}
/*
 *	@brief		ダンジョン終了集計
 *  @param[in]	int dungeonID
 *  @param[in]	bool isEventDay
 */
void InAction_Dungeon::CalculationDungeon(int dungeonID, bool isEventDay) {
	auto& world = WorldProgressManager::GetInstance();
	// ボスの討伐フラグ
	if (CharacterManager::GetInstance().GetBossDeathFlag()) world.SetIsBossDefeated(dungeonID);
	// お宝IDの取得
	int treasureID = floorProcessor.GetHoldTreasureID();
	if (treasureID == -1) return;

	if (isEventDay) {
		for (auto& eventID : treasureIDList[GameConst::EVENT_TREASURE_INDEX]) {
			if (treasureID != eventID) continue;

			world.ProcureEventTreasure(dungeonID, treasureID);
			return;
		}
		world.ProcureNewTreasure(dungeonID, treasureID);
	}
	else {
		world.ProcureNewTreasure(dungeonID, treasureID);
	}
}
