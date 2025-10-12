/*
 *	@file	DateBase.h
 *	@author	Seki
 */

#ifndef _DATE_BASE_H_
#define _DATE_BASE_H_

#include <functional>
#include <vector>
#include <memory>

/*
 *  日付共通インターフェース
 */
class DateBase {
public:
    /*
     *  デストラクタ
     */
    virtual ~DateBase() = default;

public:
    /*
     *  進行処理コールバック
     */
    std::function<void()> onAdvance;
    /*
     *  進行処理
     */
    virtual void Advance() = 0;
    /*
     *  終了フラグの取得
     */
    virtual bool IsFinished() const = 0;
}; 
#endif // !_DATE_BASE_H_

