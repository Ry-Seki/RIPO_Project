/*
 *	@file StageManager.h
 *  @author oorui
 */

#ifndef _STAGEMANAGER_H_
#define _STAGEMANAGER_H_


#include "../Singleton.h"
#include "../Engine.h"
#include "../Stage/Stage.h"
#include <memory>
#include <string>

 /*
  *	ステージ全体の管理
  */
class StageManager : public Singleton<StageManager> {
	// フレンド宣言
	friend class Singleton<StageManager>;

private:

	Engine* pEngine;                 // ゲームエンジン参照
	std::unique_ptr<Stage> pStage;   // 現在のステージ *スマートポインタにより自動的に削除されやーつ


	StageManager();
	~StageManager() = default;

public:

	/*
	 *  初期化
	 */
	void Initialize(Engine* pEngine);

	/*
	 *  ステージの読み込み
	 */
	void LoadStage(const std::string& stageCsvPath);

	/*
	 *  更新
	 */
	void Update();

	/*
	 *  描画
	 */
	void Render();

	/*
	 *  終了
	 */
	void Execute();


	/*
	 *  現在のステージの取得
	 */
	Stage* GetStage() const { return pStage.get(); }

};


#endif // !_STAGEMANAGER_H_

