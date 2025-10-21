/*
 *	@file	SelectionManager.h
 *	@author	Seki
 */

#ifndef _SELECTION_MANAGER_H_
#define _SELECTION_MANAGER_H_

#include "../../Singleton.h"
#include "SelectionBase.h"
#include "Dungeon/SelectionDungeon.h"

#include <memory>
/*
 *	各選択管理クラス
 */
class SelectionManager : public Singleton<SelectionManager>{
	// フレンドクラス宣言
	friend class Singleton<SelectionManager>;

private:
	SelectionPtr selectionBase;		// 選択クラスのオリジナル
	SelectionPtr currentSelection;	// 現在の選択肢
	SelectionPtr prevSelection;		// ひとつ前の選択肢
	bool inputHandle = false;		// 入力フラグ
	bool isActive = false;			// 処理有効フラグ

private:
	/*
	 *	コンストラクタ
	 */
	SelectionManager() : selectionBase(std::make_shared<SelectionDungeon>()) {};
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
};
#endif // !_SELECTION_MANAGER_H_
