/*
 *	@file	ActionPartTime.h
 *	@author	Seki
 */

#ifndef _ACTION_PART_TIME_H_
#define _ACTION_PART_TIME_H_

#include "../DayActionBase.h"

#include <memory>

/*
 *	アルバイトアクションクラス
 */
class ActionPartTime : public DayActionBase {
private:
    int incomeValue = 0;        // 収入
    float timer = 0.0f; 
    float limitTime = 3.0f;

public:
	/*
	 *	@brief  コンストラクタ
	 */
	ActionPartTime() : DayActionBase("アルバイト") {}
	/*
	 *	デストラクタ
	 */
	~ActionPartTime() override {}

public:
    /*
     *  初期化処理
     */
    void Initialize(Engine& engine) override;
    /*
     *  ロード済みのデータをセット(コールバック)
     */
    void SetupData(Engine& engine) override;
    /*
     *  更新処理
     */
    void Update(Engine& engine, float deltaTime) override;
    /*
     *  描画処理
     */
    void Render() override;
    /*
     *  破棄処理
     */
    void Teardown() override;

public:
    /*
     *  @brief      収入の設定
     *  @param[in]  const int setIncomeValue   収入
     */
    inline void SetIncomeValue(const int setIncomeValue) { incomeValue = setIncomeValue; }
};
#endif // !_ACTION_PART_TIME_H_