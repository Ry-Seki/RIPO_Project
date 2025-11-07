/*
 *	@file	GameObjectManager.h
 *  @author	Riku
 */
#ifndef _GAMEOBJECTMANAGER_H_
#define _GAMEOBJECTMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"
#include <mutex>

/*
 *	ゲームオブジェクトの管理クラス
 */
class GameObjectManager : public Singleton<GameObjectManager> {
	// フレンド宣言
	friend class Singleton<GameObjectManager>;
private:
	Engine* engine;
	GameObjectList unuseObjectList;
	GameObjectList useObjectList;
	std::mutex unuseMutex;

	const size_t CREATE_OBJECT_COUNT= 64;	// 事前に生成する数

private:
	GameObjectManager();
	~GameObjectManager() = default;

public:
	/*
	 *	初期化
	 */
	void Initialize(Engine& setEngine);
	/*
	 *	ID指定でのオブジェクトリストに戻す処理
	 *  @param[in]	int ID	オブジェクトの識別ID
	 *  @author		Seki
	 */
	void ReturnGameObjectList(int ID);

public:
	/*
	 *	未使用オブジェクト取得
	 */
	GameObjectPtr GetUnuseObject();
	/*
	 *	ID指定のオブジェクト取得
	 *	@param[in]	int ID	オブジェクトの識別ID
	 */
	GameObjectPtr GetUseObject(int ID);
	/*
	 *	オブジェクトのリセット
	 *  @param[in]	GameObjectPtr resetObject	リセット対象オブジェクト
	 */
	void ResetObject(GameObjectPtr resetObject);
	/*
	 *	オブジェクトのリセット
	 *	@param[in]	GameObject*	resetObject		リセット対象オブジェクト
	 *  @author		Seki
	 */
	void ResetObject(GameObject* resetObject);
};

#endif // !_GAMEOBJECTMANAGER_H_
