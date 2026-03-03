/*
 *	@file	LoadEffect.h
 *	@author	Seki
 */

#ifndef _LOAD_EFFECT_H_
#define _LOAD_EFFECT_H_

#include "../LoadBase.h"

#include <cassert>

/*
 *	@brief	エフェクトの読み込みクラス
 */
class LoadEffect : public LoadBase {
private:
	int effectHandle = -1;		// エフェクトハンドル
	float magnification = -1;	// エフェクトの拡大率
public:
	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	const std::string& setFilePath		ファイルパス
	 */
	explicit LoadEffect(const std::string& setFilePath) : LoadBase(setFilePath), magnification(1.0f){}
	/*
	 *	@brief		デストラクタ
	 */
	~LoadEffect() override;

public:
	/*
	 *	@brief	読み込み処理
	 */
	void Load() override;

public:
	/*
	 *	@brief		キャッシュするリソースか判定
	 *	@return		bool
	 */
	inline bool IsCash() override { return true; }
	/*
	 *	@brief		エフェクトハンドルの取得
	 *	@return		int
	 */
	inline int GetHandle() const { return effectHandle; }
};
#endif // !_LOAD_EFFECT_H_