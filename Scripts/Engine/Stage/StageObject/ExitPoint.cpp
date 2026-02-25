/*
 *	@file	ExitPoint.h
 *  @author kuu & oorui
 */

#include "ExitPoint.h"
#include "../../Fade/FadeFactory.h"
#include "../../GameConst.h"
#include "../../System/Money/MoneyManager.h"
#include "../../Component/Character/PlayerComponent.h"
#include "../../Component/Character/ArmActionComponent.h"
#include "../../Stage/StageObject/StageObjectBase.h"
#include "../../Component/Character/CharacterUtility.h"
#include "../../Manager/EffectManager.h"
#include "../../Menu/System/MenuConfirm.h"
#include "../../Menu/MenuManager.h"
#include "../../Input/InputUtility.h"
#include "../../Stage/StageUtility.h"
#include "../../Fade/FadeManager.h"

using namespace CharacterUtility;
using namespace InputUtility;
using namespace StageUtility;

ExitPoint::ExitPoint()
	: effectName("GoalEffect")
	, viewRadius(5000.0f)
	, exitTriger(false)
	, pViewingEffect(nullptr)
	, onTrigger(false) {
}

/*
 *	更新
 */
void ExitPoint::Update(float deltaTime) {
	// プレイヤーの取得
	auto player = GetPlayer();
	if (!player)return;

	// プレイヤーと出口の距離
	float distance = Distance(player->position, GetOwner()->position);

	// エフェクト再生
	if (distance < viewRadius) {
		if (!pViewingEffect) {
			pViewingEffect = EffectManager::GetInstance().Instantiate(
				effectName,				// 再生したいエフェクトの名前
				GetOwner()->position	// 再生する位置
			);
		}
	}
	else {
		// エフェクトが存在すれば停止
		if (pViewingEffect) {
			pViewingEffect->EffectStop();
			pViewingEffect = nullptr;
		}

	}

}

// 出口の衝突イベント
void ExitPoint::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
	// 既にプレイヤーと衝突していたら処理しない
	if (exitTriger || other->GetOwner()->name != GameConst::_CREATE_POSNAME_PLAYER)
		return;

	if (!onTrigger) {
		onTrigger = true;
		// プレイヤーの入力を停止
		SetActionMapIsActive(GameEnum::ActionMap::PlayerAction, false);
		SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);

		// 確認UIを表示
		// Menuの取得
		auto& menu = MenuManager::GetInstance();
		auto confirm = menu.GetMenu<MenuConfirm>();
		confirm->SetCallback([this, &menu](GameEnum::ConfirmResult result) {
			if (result == GameEnum::ConfirmResult::Yes) {
				// 衝突済みにする
				exitTriger = true;

				// エフェクト停止
				pViewingEffect->EffectStop();

				auto player = GetOwner()->GetComponent<PlayerComponent>();
				if (!player)return;
				auto treasure = player->GetOwner()->GetComponent<ArmActionComponent>()->GetLiftObject();
				if (!treasure)return;
				auto treasureID = treasure->GetComponent<StageObjectBase>();
				if (!treasureID)return;

				MoneyManager::GetInstance().AddTreasureMoney(treasureID->GetTreasureID());
			}
			else if (result == GameEnum::ConfirmResult::No) {
				// フェード
				FadeBasePtr fadeout = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
				FadeManager::GetInstance().StartFade(fadeout);
				onTrigger = false;
				// スタート位置に戻す
				auto player = GetPlayer();
				if (!player)return;
				// 位置の設定
				player->position = GetStartPos();
				SetActionMapIsActive(GameEnum::ActionMap::PlayerAction, true);
			}
			menu.CloseTopMenu();
			});
		menu.OpenMenu<MenuConfirm>();
	}
}

// モデルの読み込み
void ExitPoint::ModelLoad(const int modelHandleBase) {

}
