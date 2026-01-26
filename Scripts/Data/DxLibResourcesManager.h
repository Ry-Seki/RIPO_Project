/*
 *	@file	DxLibResourcesManager.h
 *  @author	Seki
 */

#ifndef _DXLIB_RESOURCES_MANAGER_H_
#define _DXLIB_RESOURCES_MANAGER_H_

#include "../Engine/Singleton.h"

#include <vector>

/*
 *	@brief	DxLibで読み込んだデータを管理するクラス
 */
class DxLibResourcesManager : public Singleton<DxLibResourcesManager>{
	friend class Singleton<DxLibResourcesManager>;

private:
	std::vector<int> graphHandleList;		// 画像ハンドルデータリスト
	std::vector<int> modelHandleList;		// モデルハンドルデータリスト
	std::vector<int> audioHandleList;		// 音源ハンドルデータリスト
	std::vector<int> effectHandleList;		// エフェクトハンドルデータリスト

public:
	/*
	 *	@brief	コンストラクタ
	 */
	DxLibResourcesManager() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~DxLibResourcesManager() override = default;

public:
	/*
	 *	@brief	片付け処理(全てのDxLibリソースを解放する)
	 */
	void Teardown();

public:
	/*
	 *	@brief		画像ハンドルの追加
	 *  @param[in]	const int setHandle		追加する画像ハンドル
	 */
	inline void AddGraphHandle(const int setHandle) { graphHandleList.push_back(setHandle); }
	/*
	 *	@brief		モデルハンドルの追加
	 *	@param[in]	const int setHandle		追加するモデルハンドル
	 */
	inline void AddModelHandle(const int setHandle) { modelHandleList.push_back(setHandle); }
	/*
	 *	@brief		音源ハンドルの追加
	 *	@param[in]	const int setHandle		追加する音源ハンドル
	 */
	inline void AddAudioHandle(const int setHandle) { audioHandleList.push_back(setHandle); }
	/*
	 *	@brief		エフェクトハンドルの追加
	 *	@param[in]	const int setHandle		追加するエフェクトハンドル
	 */
	inline void AddEffectHandle(const int setHandle) { effectHandleList.push_back(setHandle); }
};

#endif // !_DXLIB_RESOURCES_MANAGER_H_