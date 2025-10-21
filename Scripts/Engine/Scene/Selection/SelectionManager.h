/*
 *	@file	SelectionManager.h
 *	@author	Seki
 */

#ifndef _SELECTION_MANAGER_H_
#define _SELECTION_MANAGER_H_

#include "SelectionBase.h"

/*
 *	各選択管理クラス
 */
class SelectionManager {
private:
	SelectionPtr selectionBase;		// 選択クラスのオリジナル
	SelectionPtr currentSelection;	// 現在の選択肢
	bool inputHandle = false;

public:
	/*
	 *	コンストラクタ
	 */
	SelectionManager() = default;
	/*
	 *	デストラクタ
	 */
	~SelectionManager() = default;

public:
	/*
	 *	更新処理
	 */
	void Update(float deltaTime);
	/*
	 *	描画処理
	 */
	void Render();

public:
	/*
	 *	ダンジョン選択
	 */
	void DungeonSelection();
	/*
	 *	トレーニング選択
	 */
	void TrainingSelection();
	/*
	 *	ショップ選択
	 */
	void ShopSelection();
	/*
	 *	アルバイト選択
	 */
	void PartTimeSelection();
};
#endif // !_SELECTION_MANAGER_H_
