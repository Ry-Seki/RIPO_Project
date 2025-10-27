/*
 *	@file	CalendarManager.h
 *	@author	Seki
 */

#ifndef _CARENDAR_MANAGER_H_
#define _CARENDAR_MANAGER_H_

#include "CalendarSystem.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../Selection/SelectionManager.h"
#include "../DayAction/ActionManager.h"

#include <DxLib.h>
#include <functional>

// 前方宣言
class Engine;
 
/*
 *  カレンダー管理クラス
 */
class CalendarManager {
private:
    std::unique_ptr<CalendarSystem> calendarSystem;     // カレンダーシステム (常に持っている想定)
    bool inputHandle = false;                           // 入力フラグ
    bool isActive = true;                               // 行動フラグ
    int elapsedDay = 0;                                 // 消費した日にち;
    std::function<void()> DungeonSelection;             // ダンジョン選択関数

    SelectionManager* selection = nullptr;              // 選択管理クラスの関数呼び出し用参照ポインタ
    ActionManager* action = nullptr;                    // 行動管理クラスの関数呼び出し用参照ポインタ

public:
    /*
     *  コンストラクタ
     */
    CalendarManager() : calendarSystem(std::make_unique<CalendarSystem>()) {}
    /*
     *  デストラクタ
     */
    ~CalendarManager() = default;

public:
    /*
     *  初期化処理
     */
    void Initialize();
    /*
     *  更新処理
     */
    void Update(Engine& engine);
    /*
     *  描画処理
     */
    void Render(); 
    /*
     *  次の日に進行処理
     */
    void NextDay();

public:
    /*
     *  一日の行動終了フラグ取得
     */
    inline bool IsDayComplete() const { return calendarSystem->GetDay().IsFinished(); }
    /*
     *  日にち更新終了処理
     */
    inline bool IsEndDayAdvance() const { return elapsedDay >= 30; }
    /*
     *  SelectionManagerの設定
     *  @param[in]  SelectionManager* setSelection      選択管理クラスのポインタ
     */
    inline void SetSelectionManager(SelectionManager* setSelection) { selection = setSelection; }
    /*
     *  ActionManagerの設定
     *  @param[in]  ActionManager* setAction            行動管理クラスのポインタ
     */
    inline void SetActionManager(ActionManager* setAction) { action = setAction; }
};

#endif // !_CARENDER_MANAGER_H_

