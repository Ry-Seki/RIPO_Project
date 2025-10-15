/*
 *	@file	ActionPartTime.h
 *	@author	Seki
 */

#ifndef _ACTION_PART_TIME_H_
#define _ACTION_PART_TIME_H_

#include "../DayActionBase.h"
#include "../../../Load/CSV/LoadCSV.h"
#include "../../../Load/LoadManager.h"

#include <memory>

/*
 *	アルバイトアクションクラス
 */
class ActionPartTime : public DayActionBase {
private:
	std::shared_ptr<LoadCSV> partTimeDialogue;      // セリフ

public:
	/*
	 *	コンストラクタ
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
    void Initialize() override;
    /*
     *  ロード済みのデータをセット(コールバック)
     */
    void Setup() override;
    /*
     *  更新処理
     */
    void Update(float deltaTime) override;
    /*
     *  描画処理
     */
    void Render() override;
};
#endif // !_ACTION_PART_TIME_H_
