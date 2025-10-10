/*
 *	@file StageManager.h
 *  @author oorui
 */

#ifndef _STAGEMANAGER_H_
#define _STAGEMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"

 /*
  *	ステージ全体の管理
  */
class StageManager : public Singleton<StageManager> {
	// フレンド宣言
	friend class Singleton<StageManager>;

private:
	Engine* engine;



private:
	StageManager();
	~StageManager() = default;

public:
	// 初期化
	void Initialize(Engine* engine);
	// 更新
	void Update();

	// 描画
	void Render();

	// 片付け
	void Execute();



};


#endif // !_STAGEMANAGER_H_

