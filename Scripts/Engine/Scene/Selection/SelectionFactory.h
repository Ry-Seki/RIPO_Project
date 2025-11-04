/*
 *	@file	SelectionFactory.h
 *	@author	Seki
 */

#ifndef _SELECTION_FACTORY_H_
#define _SELECTION_FACTORY_H_

#include "SelectionBase.h"
#include "Dungeon/SelectionDungeon.h"

#include <memory>
#include <string>

enum class SelectionType {
	Dungeon,
	Training,
	Shop,
	PartTime
};

/*
 *	各選択の生成をするファクトリークラス
 */
class SelectionFactory {
public:
	/*
	 *	選択生成
	 */
	static SelectionPtr CreateSelection(SelectionType selection) {
		switch (selection) {
			case SelectionType::Dungeon: return std::make_shared<SelectionDungeon>();

			case SelectionType::Training:
				break;
			case SelectionType::Shop:
				break;
			case SelectionType::PartTime:
				break;
		}
	}
};

#endif // !_SELECTION_FACTORY_H_
