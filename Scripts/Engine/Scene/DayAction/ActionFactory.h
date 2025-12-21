/*
 *	@file	ActionFactory.h
 *	@author	Seki
 */

#ifndef _ACTION_FACTORY_H_
#define _ACTION_FACTORY_H_

#include "../../GameEnum.h"

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

// 前方宣言
class DayActionBase;

 /*
  *  @brief	各アクションの生成するファクトリクラス
  */
class ActionFactory {
public:		// 別名定義
	using CreateFuntion = std::function<std::shared_ptr<DayActionBase>()>;

private:
	static std::unordered_map<GameEnum::ActionType, CreateFuntion> actionMap;		// 行動クラスマップ

public:
	/*
	 *	@brief		行動クラスを全て登録
	 */
	static void RegisterAll();
	/*
	 *	@brief		行動クラス登録
	 *  @param[in]	ActionType type
	 *  @param[in]	CreateFuntion function
	 */
	static void Register(GameEnum::ActionType type, CreateFuntion function);
	/*
	 *	@brief		行動クラス生成
	 *  @param[in]	ActionType type
	 *  @return		std::shared_ptr<SelectionBase>
	 */
	static std::shared_ptr<DayActionBase> CreateAction(GameEnum::ActionType type);
};

#endif // !_ACTION_FACTORY_H_
