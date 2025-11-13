/*
 *	@file	LoadModel.h
 *	@author	Seki
 */

#ifndef _LOAD_MODEL_H_
#define _LOAD_MODEL_H_

#include "../LoadBase.h"

#include <DxLib.h>

/*
 *	DxLibの3Dモデル読み込みクラス
 */
class LoadModel : public LoadBase {
private:
	int modelHandle = -1;	// モデルハンドル

public:
	/*
	 *	コンストラクタ
	 *	param[in]	const std::string& setFilePath		ファイルパス
	 */
	explicit LoadModel(const std::string& setFilePath) : LoadBase(setFilePath) {}
	/*
	 *	デストラクタ
	 */
	~LoadModel() override { MV1DeleteModel(modelHandle); }

public:
	/*
	 *	読み込み処理
	 */
	void Load() override;

public:
	/* 
	 *	モデルハンドルの取得
	 *	return int
	 */
	inline int GetHandle() const { return modelHandle; }
};

#endif // !_LOAD_MODEL_H_
