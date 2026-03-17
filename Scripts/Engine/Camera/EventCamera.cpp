/*
 *	@file	EventCamera.cpp
 *	@author	Riku
 */

#include "EventCamera.h"
#include "MoveCommand.h"
#include "WaitCommand.h"
#include "RotateCommand.h"
#include "../Component/CameraComponent.h"
#include "../Input/InputUtility.h"

#include "../Component/Character/CharacterUtility.h"

using namespace InputUtility;
using namespace CharacterUtility;

EventCamera::EventCamera() 
	: eventNum(GameEnum::CameraEvent::Invalid)
	, commandCount(0)

	,CHANGE_VIEW_EVENT_MOVE_TIME(0.8f)
	,DEAD_EVENT_ROTATE_VALUE(0.7f)
	,DEAD_EVENT_ROTATE_TIME(0.1f)
	,DEAD_EVENT_MOVE_TIME(0.3f)
	,DEAD_EVENT_WAIT_TIME(2.0f)
{}

/*
 *	初期化処理
 */
void EventCamera::Initialize(GameObject* camera, GameEnum::CameraEvent setEvent) {
	eventNum = setEvent;
	commandCount = 0;
	
	// FPS→TPS遷移イベントの設定
	ChangeViewEventSetting(camera);

	// 死亡イベントの設定
	DeadEventSetting(camera);
}

/*
 *	更新処理
 */
void EventCamera::Update(GameObject* camera, float deltaTime) {
	// 初期化で渡されたイベントのコマンドを順番に再生
	auto& event = events[eventNum];
	auto& command = event.commands[commandCount];
	switch (command->state) {
	case CameraCommand::CommandState::Waiting:
		command->Start(camera);
		break;
	case CameraCommand::CommandState::Playing:
		command->Update(camera, deltaTime);
		break;
	case CameraCommand::CommandState::Finished:
		commandCount += 1;
		break;
	}

	// 全てのコマンドが生成し終わったら
	auto size = event.commands.size();
	if (size == commandCount) {
		// 全て再生が終わったらコマンドの初期化
		for (auto& command : event.commands) {
			command->state = CameraCommand::CommandState::Waiting;
		}
		// 終了時処理を呼ぶ
		event.onFinished();
	}
}

/*
 *	FPS→TPS遷移イベントの設定
 */
void EventCamera::ChangeViewEventSetting(GameObject* camera) {
	auto event = events[GameEnum::CameraEvent::ChangeView];
	event.commands.clear();
	
	// カメラの移動先を計算
	auto pos = camera->GetComponent<CameraComponent>()->GetTPSCameraPosition(GetPlayer());
	// 計算された移動先へ向かうコマンド
	event.commands.push_back(std::make_shared<MoveCommand>(CHANGE_VIEW_EVENT_MOVE_TIME, pos));
	
	// 終了時処理
	event.onFinished = [camera]() {
		// カメラステートをTPSに
		camera->GetComponent<CameraComponent>()->SetState(GameEnum::CameraState::TPS);
		// プレイヤーの入力を取る
		SetActionMapIsActive(GameEnum::ActionMap::PlayerAction, true);
		};
}

/*
 *	死亡イベントの設定
 */
void EventCamera::DeadEventSetting(GameObject* camera) {
	auto player = GetPlayer()->GetComponent<PlayerComponent>();
	auto event = events[GameEnum::CameraEvent::Dead];
	event.commands.clear();
	
	// 回転の開始角度
	Vector3 startRot = camera->rotation;
	startRot.x = 0;
	// 回転先の角度
	Vector3 targetRot = camera->rotation;
	targetRot.x = DEAD_EVENT_ROTATE_VALUE;
	// 回転コマンド
	event.commands.push_back(std::make_shared<RotateCommand>(DEAD_EVENT_ROTATE_TIME, startRot, targetRot));
	
	// カメラの移動先を計算
	auto pos = camera->GetComponent<CameraComponent>()->GetTPSCameraPosition(GetPlayer());
	// 移動コマンド
	event.commands.push_back(std::make_shared<MoveCommand>(DEAD_EVENT_MOVE_TIME, pos));
	
	// 待機コマンド
	event.commands.push_back(std::make_shared<WaitCommand>(DEAD_EVENT_WAIT_TIME));
	
	// 終了時処理
	event.onFinished = [player, camera]() {
		// プレイヤー死亡
		player->SetIsDead(true);
		camera->GetComponent<CameraComponent>()->SetState(GameEnum::CameraState::Invalid);
		};
}
