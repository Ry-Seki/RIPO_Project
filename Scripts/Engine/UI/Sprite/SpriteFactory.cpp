/*
 *  @file   SpriteFactory.cpp
 *  @author Seki
 */

#include "SpriteFactory.h"
#include "../../../Data/UI/Sprite/SpriteInfo.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Load/LoadManager.h"
#include "../../UI/Sprite/Sprite.h"

/*
 *	@brief		スプライトの生成
 *	@param[in]	const SpriteInfo& info
 *	@return		std::shared_ptr<Sprite>
 */
std::shared_ptr<Sprite> SpriteFactory::CreateSprite(const SpriteInfo& info) {
    if (info.resourcePathList.empty()) return nullptr;

    auto sprite = std::make_shared<Sprite>();
    sprite->SetRect(info.rect);

    auto& load = LoadManager::GetInstance();

    int resourceCount = static_cast<int>(info.resourcePathList.size());
    auto loadedCount = std::make_shared<int>(0);

    for (const auto& path : info.resourcePathList) {
        auto texture = load.LoadResource<LoadSprite>(path);
        // コールバック回数を減らす処理
        load.SetOnComplete([sprite, texture, loadedCount, resourceCount]() {
            sprite->SetGraphHandle(texture->GetHandle());
            (*loadedCount)++;

            if (*loadedCount == resourceCount) sprite->Setup();
        });
    }

    return sprite;
}
