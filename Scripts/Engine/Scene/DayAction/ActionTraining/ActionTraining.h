/*
 *	@file	ActionTraining.h
 *	@author	Seki
 */

#ifndef _ACTION_TRAINING_H_
#define _ACTION_TRAINING_H_

#include "../DayActionBase.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/CSV/LoadCSV.h"

#include <memory>

/*
 *	@brief  トレーニングアクションクラス
 */
class ActionTraining : public DayActionBase {
private:
    std::shared_ptr<LoadCSV> trainingDialogue;      // セリフデータ
    int actionNum = -1;                             // アクション番号

private:
    float timer = 0.0f;
    float limitTime = 3.0f;

public:
    /*
     *  @brief  コンストラクタ
     */
    ActionTraining() : DayActionBase("Training") {}
    /*
     *  @brief  デストラクタ
     */
    ~ActionTraining() override {}

public:
    /*
     *  @brief  初期化処理
     */
    void Initialize(Engine& engine) override;
    /*
     *  @brief  準備前処理
     */
    void Setup(Engine& engine) override;
    /*
     *  @brief  ロード済みのデータをセット(コールバック)
     */
    void SetupData(Engine& engine);
    /*
     *  @brief  更新処理
     */
    void Update(Engine& engine, float deltaTime) override;
    /*
     *  @brief  描画処理
     */
    void Render() override;
    /*
     *  @brief  破棄処理
     */
    void Teardown() override;

public:
    /*
     *  @brief      アクション番号の設定
     *  @param[in]  int setValue            アクション番号
     */
    inline void SetActionNum(int setValue) { actionNum = setValue; }
};

#endif // !_ACTION_TRAINING_H_
