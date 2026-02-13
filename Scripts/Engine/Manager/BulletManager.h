/*
 *	@file	BulletManager.h
 *  @author	Riku
 */

#ifndef _BULLETMANAGER_H_
#define _BULLETMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"
#include "../Load/Model/LoadModel.h"
#include "../Component/Character/BulletComponent.h"

/*
 *	弾の管理クラス
 */
class BulletManager : public Singleton<BulletManager> {
	// フレンド宣言
	friend class Singleton<BulletManager>;

private:
	Engine* engine;
	std::shared_ptr<LoadModel> bulletModel;

	const std::string BULLET_NAME;  // 弾の名前
	const Vector3 BULLET_AABB_MIN;	// 弾のAABBMin
	const Vector3 BULLET_AABB_MAX;	// 弾のAABBMax


private:
	BulletManager();
	~BulletManager() = default;

private:
	/*
	 *	弾生成
	 *	@param	name		弾の名前
	 *	@param	position	生成位置
	 *	@param	rotation	生成角度
	 *	@param	AABBMin		AABBの各軸における最小値
	 *	@param	AABBMax		AABBの各軸における最大値
	 *  @return	BulletComponentPtr
	 */
	BulletComponentPtr GenerateBullet(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax);

public:
	/*
	 *	初期化
	 *	@param	setEngine	エンジン
	 */
	void Initialize(Engine& setEngine);
	
	/*
	 *	射撃
	 *	@param	position	生成位置
	 *	@param	rotation	生成角度
	 *  @param	scale		サイズ
	 *	@param	direction	射撃方向
	 *  @param	shotOwner	射撃者
	 *  @param	speed		弾の速度
	 *  @param	damage		弾のダメージ
	 */
	void BulletShot(
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& scale,
		const Vector3& direction,
		GameObject* shotOwner,
		const float speed,
		const float damage);

};

#endif // !_BULLETMANAGER_H_
