/*
 *	@file	SelectionManager.h
 *	@author	Seki
 */

#ifndef _SELECTION_MANAGER_H_
#define _SELECTION_MANAGER_H_

#include "SelectionBase.h"
#include "SelectionFactory.h"
#include "../DayAction/ActionManager.h"

#include <memory>
#include <functional>

/*
 *	各選択管理クラス
 */
class SelectionManager{
private:
	SelectionPtr currentSelection;				// 現在の選択肢
	SelectionPtr prevSelection;					// ひとつ前の選択肢
	bool inputHandle = false;					// 入力フラグ
	bool isActive = false;						// 処理有効フラグ

	ActionManager* actionManager = nullptr;		// 行動管理クラスの関数呼び出し用参照ポインタ
	std::function<void()> ActiveDungeon;		// 行動管理クラスのダンジョン解放関数

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
	void Update(Engine& engine, float deltaTime);
	/*
	 *	描画処理
	 */
	void Render();

public:
	/*
	 *	ダンジョン選択
	 */
	void DungeonSelection(Engine& engine);
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

public:
	/*
	 *	処理有効フラグの取得
	 */
	inline bool IsActive() const { return isActive; }
	/*
	 *  ActionManagerの設定
	 *  @param[in]  ActionManager* setAction            行動管理クラスのポインタ
	 */
	inline void SetActionManager(ActionManager* setAction) { actionManager = setAction; }
	/*
	 *	ActionManagerのダンジョン関数の設定
	 *	@param[in]	std::function<void()> setActiveDungeon	行動管理クラスのコールバック関数
	 */
	inline void SetActiveDungeon(std::function<void()> setActiveDungeon) { ActiveDungeon = setActiveDungeon; }

};
#endif // !_SELECTION_MANAGER_H_
