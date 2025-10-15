/*
 *	@file	DebugScene.cpp
 *	@author	Seki
 */

#include "DebugScene.h"
#include "../Manager/CameraManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/CharacterManager.h"

/*
 *	初期化処理
 */
void DebugScene::Initialize(Engine& engine) {
	GameObjectManager::GetInstance().Initialize(engine);
	CharacterManager::GetInstance().Initialize(engine);
	CameraManager::GetInstance().CreateCamera("camera", { 0, 0, 0 }, { 0, 0, 0 });
	CharacterManager::GetInstance().CreatePlayer("player", { 0, 0, 0 }, { 0, 0, 0 });
}
/*
 *	更新処理
 */
void DebugScene::Update(Engine& engine, float deltaTime) {
	Scene::Update(engine, deltaTime);
}
/*
 *	描画処理
 */
void DebugScene::Render() {
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
			pos1 = VGet(0,0,0);
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
}
