/*
 *	@file	EffectManager.cpp
 *  @author	oorui
 */

#include "EffectManager.h"
#include "EffekseerForDXLib.h"
#include "../Component/EffectComponent.h"
#include "../Load/JSON/LoadJSON.h"
#include "../Scripts/Data/DxLibResourcesManager.h"

 /*
  *	コンストラクタ
  */
EffectManager::EffectManager()
	:engine(nullptr)
	, effectResourceMap()
	, pEffectList()
	, isLoop(true) {
}

/*
 *	初期化
 */
void EffectManager::Initialize(Engine& setEngine) {
	engine = &setEngine;
	if (json.empty())return;
	// エフェクトをまとめてロード
	LoadEffects();

}

/*
 *	エフェクトの読み込み
 *  @param	filePath				ファイルパス
 *  @param	name					エフェクトに設定する名前
 *  @param  magnification = 1.0f	拡大率
 */
void EffectManager::Load(std::string filePath, std::string name, float magnification, bool loop) {

	// すでに登録済みなら何もしない
	if (effectResourceMap.contains(name)) {
		return;
	}

	// エフェクト読み込み
	int res = LoadEffekseerEffect(filePath.c_str(), magnification);

	// 読み込み失敗チェック
	if (res < 0) {
		return;
	}

	// 名前をキーに登録
	// 名前 , EffectResource{ ハンドル , ループ }
	effectResourceMap.emplace(name, EffectResource{ res,loop });

	// エフェクトのハンドルをDxLibのリストに追加
	DxLibResourcesManager::GetInstance().AddEffectHandle(res);
}

/*
 *	エフェクトの発生
 */
EffectComponent* EffectManager::Instantiate(std::string name, Vector3 _pos) {
	// 登録されているか検索
	auto itr = effectResourceMap.find(name);
	if (itr == effectResourceMap.end()) {
		return nullptr;
	}

	// エフェクトのハンドルを取得
	auto& data = itr->second;

	EffectComponent* pEffect = new EffectComponent(data.handle, data.loop);
	pEffect->SetPosition(_pos);
	pEffect->SetVisible(true);
	pEffectList.push_back(pEffect);

	return pEffect;
}

/*
 *	エフェクトの更新
 */
void EffectManager::Update() {

	// エフェクト再生
	for (auto pEffe : pEffectList) {
		if (pEffe == nullptr || !pEffe->IsVisile())continue;
		// エフェクト再生
		pEffe->EffectRenderer();
	}

	// エフェクト更新
	UpdateEffekseer3D();

	// エフェクト終了判定
	for (auto pEffe : pEffectList) {
		if (pEffe == nullptr || !pEffe->IsVisile())continue;
		// エフェクト停止
		pEffe->EffectAllStop();
	}


	// エフェクトのリストから再生されていないエフェクトを削除
	std::erase_if(pEffectList, [](EffectComponent* e) {
		if (!e->IsVisile()) {
			delete e;
			return true;
		}
		return false;
		}
	);


}

/*
 *	描画処理
 */
void EffectManager::Render() {
	for (auto pEffe : pEffectList) {
		if (pEffe == nullptr || !pEffe->IsVisile())
			continue;
		// エフェクトの描画
		pEffe->Render();
	}

}


/*
 *	エフェクトを一括で読み込む
 */
void EffectManager::LoadEffects() {
	// effects 配列を取得
	const auto& effectsArray = json["effects"];

	// 配列の各要素を順に処理する
	for (const auto& eff : effectsArray) {
		if (!eff.contains("file") || !eff.contains("name")) continue;

		// 各要素をキャッシュ
		std::string filePath = eff["file"].get<std::string>();	// ファイルパス
		std::string name = eff["name"].get<std::string>();		// エフェクトの名前
		float magnification = eff.value("magnification", 1.0f);	// 拡大率
		isLoop = eff["loop"].get<bool>();					// ループ可否

		// エフェクトをすべてロード
		Load(filePath, name, magnification, isLoop);

	}
}