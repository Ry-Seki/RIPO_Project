/*
 *	@file	SelectionManager.h
 *	@author	Seki
 */

#ifndef _SELECTION_MANAGER_H_
#define _SELECTION_MANAGER_H_

#include "../../GameEnum.h"

#include <memory>
#include <unordered_map>
#include <functional>

// 前方宣言
class Engine;
class SelectionBase;
class ActionManager;

/*
 *	@brief	各選択管理クラス
 */
class SelectionManager{
private:
	std::unordered_map<GameEnum::ActionType,
		std::shared_ptr<SelectionBase>> selectionMap;	// 選択クラスリスト
	std::shared_ptr<SelectionBase> currentSelection;	// 現在の選択肢
	std::shared_ptr<SelectionBase> prevSelection;		// ひとつ前の選択肢
	bool inputHandle = false;							// 入力フラグ
	bool isActive = false;								// 処理有効フラグ

	ActionManager* actionManager = nullptr;				// 行動管理クラスの関数呼び出し用参照ポインタ
	std::function<void()> ActiveDungeon;				// 行動管理クラスのダンジョン解放関数

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
	void TrainingSelection(Engine& engine);
	/*
	 *	ショップ選択
	 */
	void ShopSelection(Engine& engine);
	/*
	 *	アルバイト選択
	 */
	void PartTimeSelection(Engine& engine);

public:
	/*
	 *	@brief		選択クラスの取得
	 *  @param[in]	ActionType type
	 *	@return		std::shared_ptr<SelectionBase>
	 */
	std::shared_ptr<SelectionBase> GetSelection(GameEnum::ActionType type, Engine& engine);
	/*
	 *	@brief		処理有効フラグの取得
	 *  @return		bool
	 */
	inline bool IsActive() const { return isActive; }
	/*
	 *  @brief		ActionManagerの設定
	 *  @param[in]  ActionManager* setAction            行動管理クラスのポインタ
	 */
	inline void SetActionManager(ActionManager* setAction) { actionManager = setAction; }
	/*
	 *	@brief		ActionManagerのダンジョン関数の設定
	 *	@param[in]	std::function<void()> setActiveDungeon	行動管理クラスのコールバック関数
	 */
	inline void SetActiveDungeon(std::function<void()> setActiveDungeon) { ActiveDungeon = setActiveDungeon; }

};
#endif // !_SELECTION_MANAGER_H_
