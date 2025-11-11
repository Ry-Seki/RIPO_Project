/*
 *	@file	BulletManager.h
 *  @author	Riku
 */

#ifndef _BULLETMANAGER_H_
#define _BULLETMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"

/*
 *	弾の管理クラス
 */
class BulletManager : public Singleton<BulletManager> {
	// フレンド宣言
	friend class Singleton<BulletManager>;

private:
	Engine* engine;
	int bulletHandle = -1;

private:
	BulletManager();
	~BulletManager() = default;

public:
	void Initialize(Engine& setEngine);
	/*
	 *	弾生成
	 *	@param	name		弾の名前
	 *	@param	position	生成位置
	 *	@param	rotation	生成角度
	 *	@param	AABBMin		AABBの各軸における最小値
	 *	@param	AABBMax		AABBの各軸における最大値
	 */
	void GenerateBullet(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax);

};

#endif // !_BULLETMANAGER_H_
