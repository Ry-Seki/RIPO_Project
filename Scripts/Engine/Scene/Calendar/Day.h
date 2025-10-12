/*
 *	@file	Day.h
 *	@author	Seki
 */

#ifndef _DAY_H_
#define _DAY_H_

#include "DateBase.h"

#include <DxLib.h>

/*
 *  一日の行動管理クラス
 */
class Day : public DateBase {
public:
    bool morningDone = false;       // 午前行動フラグ
    bool afternoonDone = false;     // 午後行動フラグ

public:
    /*
     *  進行処理
     */
    void Advance() override;
    /*
     *  一日の行動終了フラグ
     */
    bool IsFinished() const override { return morningDone && afternoonDone; }

public:
    /*
     *  ダンジョン処理
     */
    void ActionDungeon();
    /*
     *  トレーニング処理
     */
    void ActionTraining();
    /*
     *  ショップ処理
     */
    void ActionShop();
    /*
     *  アルバイト処理
     */
    void ActionPartTime();
    /*
     *  半日終了
     *  @param[in]  const char* actionName      行動名
     */
    void AdvanceHalf(const char* actionName);
    /*
     *  一日消費
     */
    void AdvanceDay(); 
    /*
     *  行動リセット
     */
    void ResetDay();
}; 
#endif // !_DAY_H_

