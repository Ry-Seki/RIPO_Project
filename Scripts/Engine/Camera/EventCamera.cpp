/*
 *	@file	EventCamera.cpp
 *	@author	Riku
 */

#include "EventCamera.h"
#include "MoveCommand.h"
#include "../Component/CameraComponent.h"
#include "../Input/InputUtility.h"

#include "../Component/Character/CharacterUtility.h"

using namespace InputUtility;
using namespace CharacterUtility;

EventCamera::EventCamera() 
	: eventNum(GameEnum::CameraEvent::Invalid)
	, commandCount(0)
{}

/*
 *	初期化処理
 */
void EventCamera::Initialize(GameObject* camera, GameEnum::CameraEvent setEvent) {
	eventNum = setEvent;
	commandCount = 0;
	auto player = GetPlayer()->GetComponent<PlayerComponent>();
	
	// FPS→TPS遷移イベント
	events[GameEnum::CameraEvent::ChangeView].commands.clear();
	Vector3 playerHeadPos = GetPlayer()->position;
	playerHeadPos.y += 310;
	auto pos = playerHeadPos - ForwardDir(camera->rotation) * 500;
	events[GameEnum::CameraEvent::ChangeView].commands.push_back(std::make_shared<MoveCommand>(0.8f, pos));
	events[GameEnum::CameraEvent::ChangeView].onFinished = [camera]() {
		// カメラステートをTPSに
		camera->GetComponent<CameraComponent>()->SetState(GameEnum::CameraState::TPS);
	};

	// 死亡イベント
	events[GameEnum::CameraEvent::Dead].commands.clear();
	events[GameEnum::CameraEvent::Dead].commands.push_back(std::make_shared<MoveCommand>(0.8f, pos));
	events[GameEnum::CameraEvent::Dead].onFinished = [player, camera]() {
		// プレイヤー死亡
		player->SetIsDead(true);
		camera->GetComponent<CameraComponent>()->SetState(GameEnum::CameraState::Invalid);
	};
}

/*
 *	更新処理
 */
void EventCamera::Update(GameObject* camera, float deltaTime) {
	// 初期化で渡されたイベントのコマンドを順番に再生
	auto command = events[eventNum].commands[commandCount];
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
	auto size = events[eventNum].commands.size();
	if (size == commandCount) {
		// プレイヤーの入力を取る
		SetActionMapIsActive(GameEnum::ActionMap::PlayerAction, true);
		// 全て再生が終わったらコマンドの初期化
		for (auto command : events[eventNum].commands) {
			command->state = CameraCommand::CommandState::Waiting;
		}
		// 終了時処理を呼ぶ
		events[eventNum].onFinished();
	}
}