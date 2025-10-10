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
    std::function<void()> onAdvance;   // 日付が進んだときのコールバック
    virtual void Advance() = 0;        // 仮想関数で進行処理
    virtual bool IsFinished() const = 0; // 終了判定
    virtual ~DateBase() = default;
}; 
#endif // !_DATE_BASE_H_

