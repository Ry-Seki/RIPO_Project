/*
 *	@file	SelectionFactory.h
 *	@author	Seki
 */

#ifndef _SELECTION_FACTORY_H_
#define _SELECTION_FACTORY_H_

#include "../../GameEnum.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

// 前方宣言
class SelectionBase;

/*
 *	@brief	各選択の生成をするファクトリークラス
 */
class SelectionFactory {
public:		// 別名定義
	using CreateFuntion = std::function<std::shared_ptr<SelectionBase>()>;

private:
	static std::unordered_map<GameEnum::ActionType, CreateFuntion> selectionMap;		// 選択クラスマップ

public:
	/*
	 *	@brief		選択クラスを全て登録
	 */
	static void RegisterAll();
	/*
	 *	@brief		選択クラス登録
	 *  @param[in]	ActionType type
	 *  @param[in]	CreateFuntion function
	 */
	static void Register(GameEnum::ActionType type, CreateFuntion function);
	/*
	 *	@brief		選択クラス生成
	 *  @param[in]	ActionType type
	 *  @return		std::shared_ptr<SelectionBase>
	 */
	static std::shared_ptr<SelectionBase> CreateSelection(GameEnum::ActionType type);
};

#endif // !_SELECTION_FACTORY_H_
