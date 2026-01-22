/*
 *	@file	LoadModel.cpp
 *	@author	Seki
 */

#include "LoadModel.h"
#include "../../../Data/DxLibResourcesManager.h"

#include <DxLib.h>

/*
 *	@brief	デストラクタ
 */
LoadModel::~LoadModel() {
	if(modelHandle != -1) MV1DeleteModel(modelHandle);
}

/*
 *	読み込み処理
 */
void LoadModel::Load() {
	// モデルの読み込み
	modelHandle = MV1LoadModel(filePath.c_str());
	if (modelHandle == -1) {
		assert(false && "モデル読み込み失敗");
		return;
	}
	// 読み込み完了
	isLoaded = true;
	DxLibResourcesManager::GetInstance().AddModelHandle(modelHandle);
}
