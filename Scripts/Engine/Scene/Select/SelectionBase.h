/*
 *	@file	SelectionBase.h
 *	@author	Seki
 */

#ifndef _SELECTION_BASE_H_
#define _SELECTION_BASE_H_

#include <functional>
#include <memory>
#include <any>

/*
 *	行動の選択肢を管理するクラス
 */
class SelectionBase {
public:
    virtual ~SelectionBase() = default;

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual bool IsActive() const = 0;
};
// 別名定義
using SelectionBasePtr = std::shared_ptr<SelectionBase>;

#endif // !_SELECTION_BASE_H_