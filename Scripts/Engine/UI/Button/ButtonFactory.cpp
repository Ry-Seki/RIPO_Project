/*
 *  @file   ButtonFactory.cpp
 *  @author Seki
 */

#include "ButtonFactory.h"
#include "SinglePressButton.h"
#include "LongPressButton.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../GameEnum.h"
#include "../../../Data/UI/Button/ButtonInfo.h"

/*
 *	@brief		ボタンを生成
 *  @param[in]	const ButtonInfo& buttonInfo
 *	@return		std::shared_ptr<UIButtonBase>
 */
std::shared_ptr<UIButtonBase> ButtonFactory::CreateButton(const ButtonInfo& buttonInfo) {
    std::shared_ptr<UIButtonBase> button;
	// ボタンの生成
	// TODO : ここをスイッチ分をできるだけ使わないようにしたい
	switch (buttonInfo.type) {
		case GameEnum::ButtonType::SinglePressButton:
			button = std::make_shared<SinglePressButton>(buttonInfo.rect);
			break;
		case GameEnum::ButtonType::LongPressButton:
			button = std::make_shared<LongPressButton>(buttonInfo.rect);
			break;
		default:
			return nullptr;
	}
	// ボタンの初期化処理
	button->Initialize();
	// 名前の登録
	button->SetName(buttonInfo.name);

	// リソース登録
	auto& load = LoadManager::GetInstance();

	for (int i = 0, max = buttonInfo.resourceList.size(); i < max; i++) {
		const auto& path = buttonInfo.resourceList[i];
		if (path.empty()) continue;

		auto sprite = load.LoadResource<LoadSprite>(path);

		load.SetOnComplete([button, sprite, i]() {
			if (!button || !sprite) return;
			button->RegisterButtonHandle(i, sprite->GetHandle());
		});
	}
	return button;
}
