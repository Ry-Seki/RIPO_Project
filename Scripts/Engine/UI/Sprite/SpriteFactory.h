/*
 *	@file	SpriteFactory.h
 *	@author	Seki
 */

#ifndef _SPRITE_FACTORY_H_
#define _SPRITE_FACTORY_H_

#include <memory>

// 前方宣言
class Sprite;
struct SpriteInfo;

/*
 *	@brief	スプライト生成クラス
 */
class SpriteFactory {
public:
	/*
	 *	@brief		スプライトの生成
	 *	@param[in]	const SpriteInfo& info
	 *	@return		std::shared_ptr<Sprite>
	 */
	static std::shared_ptr<Sprite> CreateSprite(const SpriteInfo& info);
};

#endif // !_SPRITE_FACTORY_H_