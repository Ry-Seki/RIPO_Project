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
 *	トレーニングアクションクラス
 */
class ActionTraining : public DayActionBase {
private:
    std::shared_ptr<LoadCSV> trainingDialogue;      // セリフデータ

public:
    /*
     *  コンストラクタ
     */
    ActionTraining() : DayActionBase("Training") {}
    /*
     *  デストラクタ
     */
    ~ActionTraining() override {}

public:
    /*
     *  初期化処理
     */
    void Initialize(Engine& engine) override;
    /*
     *  ロード済みのデータをセット(コールバック)
     */
    void Setup(Engine& engine) override;
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
};

#endif // !_ACTION_TRAINING_H_
