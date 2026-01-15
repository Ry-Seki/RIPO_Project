/*
 *	@file	InAction_PartTime.h
 *	@author	Seki
 */

#ifndef _IN_ACTION_PART_TIME_H_
#define _IN_ACTION_PART_TIME_H_

#include "../InActionBase.h"

#include <vector>
#include <memory>

// 前方宣言
class MiniGameBase;

class InAction_PartTime : public InActionBase {
public:
	using MiniGamePtr = std::shared_ptr<MiniGameBase>;

private:
	std::vector<MiniGamePtr> miniGameList;
	MiniGamePtr miniGame = nullptr;

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	準備前処理
	 */
	void Setup() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	片付け処理
	 */
	void Teardown() override;

};

#endif // !_IN_ACTION_PART_TIME_H_