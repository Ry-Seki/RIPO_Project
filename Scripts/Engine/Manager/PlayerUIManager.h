/*
 *	@file	PlayerUIManager.h
 *	@author	Riku
 */

#ifndef _PLAYERUIMANAGER_H_
#define _PLAYERUIMANAGER_H_

#include "../Singleton.h"
#include "../UI/PlayerUI/ReticleUI.h"
#include <memory>

/*
 *	プレイヤーのUIを管理するクラス
 */
class PlayerUIManager : Singleton<PlayerUIManager> {
	friend class Singleton<PlayerUIManager>;
private:
	std::shared_ptr<ReticleUI> reticle;

private:
	PlayerUIManager() = default;
	~PlayerUIManager() = default;

public:
	/*
	 *	初期化
	 */
	void Initialize();

	/*
	 *	描画
	 */
	void Render();

};

#endif // !_PLAYERUIMANAGER_H_
