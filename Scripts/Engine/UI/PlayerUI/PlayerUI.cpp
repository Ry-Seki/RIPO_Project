/*
 *	@file	PlayerUI.cpp
 *	@autor	Riku
 */

#include "PlayerUI.h"
#include "ReticleUI.h"
#include "AmmoCountUI.h"
#include "ReloadGaugeUI.h"
#include "ResistTimeGaugeUI.h"

/*
 * 初期化
 */
void PlayerUI::Initialize(Engine& engine) {
	// UIの数生成
	UIs.push_back(std::make_shared<ReticleUI>());
	UIs.push_back(std::make_shared<AmmoCountUI>());
	UIs.push_back(std::make_shared<ReloadGaugeUI>());
	UIs.push_back(std::make_shared<ResistTimeGaugeUI>());
	
	// UIの初期化
	for (auto ui : UIs) {
		ui->Initialize();
	}
}

/*
 *	@brief	更新処理
 */
void PlayerUI::Update(Engine& engine, float unscaledDeltaTime) {
	// UIの更新
	for (auto ui : UIs) {
		ui->Update(unscaledDeltaTime);
	}
}

/*
 *	@brief	メニューを開く
 */
void PlayerUI::Open() {
	isVisible = true;
	isInteractive = true;
	isStart = false;
}

/*
 *	描画
 */
void PlayerUI::Render() {
	// UIの描画
	for (auto ui : UIs) {
		ui->Render();
	}
}
