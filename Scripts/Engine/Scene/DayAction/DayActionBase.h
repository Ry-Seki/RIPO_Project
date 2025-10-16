/*
 *	@file	DayActionBase.h
 *	@author	Seki
 */

#ifndef _DAY_ACTION_BASE_H_
#define _DAY_ACTION_BASE_H_

#include <string>
#include <memory>
#include <iostream>
#include "../../Load/LoadManager.h"
#include "../../Load/LoadBase.h"

 /*
  *  1日単位の行動の基底クラス
  */
class DayActionBase {
protected:
    std::string name;                   // 行動名
    bool isComplete = false;            // 行動完了フラグ

public:
    explicit DayActionBase(const std::string& setName) : name(setName) {}
    virtual ~DayActionBase() = default;

    /*
     *  初期化処理(データのロード登録)
     */
    virtual void Initialize() = 0;
    /*
     *  ロード済みのデータをセット(コールバック)
     */
    virtual void Setup() = 0;
    /*
     *  更新処理
     */
    virtual void Update(float deltaTime) = 0;
    /*
     *  描画処理
     */
    virtual void Render() = 0;

public:
    /*
     *  行動完了フラグ
     *  return  bool
     */
    inline bool IsComplete() const { return isComplete; }
    /*
     *  行動名の取得
     *  return string
     */
    inline const std::string& GetName() const { return name; }
};
// 別名定義
using DayActionPtr = std::shared_ptr<DayActionBase>;

#endif // !_DAY_ACTION_BASE_H_
