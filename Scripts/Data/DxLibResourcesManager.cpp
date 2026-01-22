/*
 *	@file	DxLibResourcesManager.cpp
 *	@author	Seki
 */

#include "DxLibResourcesManager.h"

#include <DxLib.h>
#include <EffekseerForDXLib.h>

/*
 *	@brief	片付け処理
 */
void DxLibResourcesManager::Teardown() {
	// エフェクトの解放
	for (auto& effect : effectHandleList) {
		DeleteEffekseerEffect(effect);
	}
	effectHandleList.clear();
	// モデルの解放
	for (auto& model : modelHandleList) {
		if(model != -1) MV1DeleteModel(model);
	}
	modelHandleList.clear();
	// 画像の解放
	for (auto& graph : graphHandleList) {
		if(graph != -1) DeleteGraph(graph);
	}
	graphHandleList.clear();
	// 音源の解放
	for (auto& audio : audioHandleList) {
		if(audio != -1) DeleteSoundMem(audio);
	}
	audioHandleList.clear();
}
