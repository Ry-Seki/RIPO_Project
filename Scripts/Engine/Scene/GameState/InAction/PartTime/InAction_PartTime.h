/*
 *	@file	InAction_PartTime.h
 *	@author	Seki
 */

#ifndef _IN_ACTION_PART_TIME_H_
#define _IN_ACTION_PART_TIME_H_

#include "../InActionBase.h"
#include "../../../../GameEnum.h"

#include <vector>
#include <memory>
#include <string>

// 前方宣言
class MiniGameBase;

class InAction_PartTime : public InActionBase {
public:
	using MiniGamePtr = std::shared_ptr<MiniGameBase>;

private:
	std::vector<MiniGamePtr> miniGameList;
	MiniGamePtr miniGame = nullptr;

	static constexpr const char* _PART_BGMPATH = "Res/Audio/BGM/Part/GameBGM.mp3";				// 倉庫番のBGMのパス
	static constexpr const char* _PART_SE01PATH = "Res/Audio/SE/SOKOBAN/ビープ音2.mp3";			// 倉庫番の壁にあたったときのSEのパス
	static constexpr const char* _PART_SE02PATH = "Res/Audio/SE/SOKOBAN/自動ドアが開く.mp3";	// 倉庫番の移動時のSEのパス
	static constexpr const char* _PART_SE03PATH = "Res/Audio/SE/SOKOBAN/決定ボタンを押す7.mp3"; // 倉庫番の指定位置についたときのSEのパス
	static constexpr const char* _SELECTPART_BGMPATH = "Res/Audio/BGM/Part/Dall_Tube_Heaven.mp3";

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

private:
	/*
	 *	@brief		ミニゲーム完了処理
	 *	@param[in]	GameEnum::MiniGameLevel level
	 */
	void CompleteMiniGame(GameEnum::MiniGameLevel level);
};

#endif // !_IN_ACTION_PART_TIME_H_