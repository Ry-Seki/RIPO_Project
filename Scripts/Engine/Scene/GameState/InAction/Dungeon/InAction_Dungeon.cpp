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
#include "../../../../Load/LoadManager.h"
#include "../../../../Component/GravityComponent.h"
#include "../../../../Audio/AudioUtility.h"

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
}
/*
 *	@brief	準備前処理
 */
void InAction_Dungeon::Setup() {
	isStart = false;
	auto& context = owner->GetOwner()->GetActionContext();
	floorProcessor.CreateFloor(context, isStart);
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
	// 出口に触れたとき
	if (exitFrag) {
		// SEの再生
		AudioUtility::PlaySE("GoalSE");
		EndDungeon();
	}
	else if (stairFrag) {
		floorProcessor.ChangeFloor();
	}
}
/*
 *	@brief	描画処理
 */
void InAction_Dungeon::Render() {
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
 *	@brief	片付け処理
 */
void InAction_Dungeon::Teardown() {
}
/*
 *	@brief	ダンジョン終了処理
 */
void InAction_Dungeon::EndDungeon() {
	floorProcessor.EndDungeon();
	owner->GetOwner()->ChageState(GameEnum::GameState::ResultAction);
}
