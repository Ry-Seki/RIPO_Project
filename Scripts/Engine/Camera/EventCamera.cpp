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
	
	events[GameEnum::CameraEvent::ChangeView].clear();
	Vector3 playerHeadPos = GetPlayer()->position;
	playerHeadPos.y += 310;
	auto pos = playerHeadPos - ForwardDir(camera->rotation) * 500;
	events[GameEnum::CameraEvent::ChangeView].push_back(std::make_shared<MoveCommand>(1, pos));
}

/*
 *	更新処理
 */
void EventCamera::Update(GameObject* camera, float deltaTime) {
	// 初期化で渡されたイベントのコマンドを順番に再生
	auto command = events[eventNum][commandCount];
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

	auto size = events[eventNum].size();
	if (size == commandCount) {
		// カメラステートをTPSに
		camera->GetComponent<CameraComponent>()->SetState(GameEnum::CameraState::TPS);
		// プレイヤーの入力を取る
		SetActionMapIsActive(GameEnum::ActionMap::PlayerAction, true);
		// 全て再生が終わったらコマンドの初期化
		for (auto command : events[eventNum]) {
			command->state = CameraCommand::CommandState::Waiting;
		}
	}
}