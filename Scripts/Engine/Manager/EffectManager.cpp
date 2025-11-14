/*
 *	@file	EffectManager.cpp
 *  @author	oorui
 */

#include "EffectManager.h"
#include "EffekseerForDXLib.h"
#include "../Component/EffectComponent.h"

 /*
  *	コンストラクタ
  */
EffectManager::EffectManager()
	:engine(nullptr)
	, effectResourceMap()
	, pEffectList() {
}

/*
 *	エフェクトの読み込み
 *  @param	filePath				ファイルパス
 *  @param	name					エフェクトに設定する名前
 *  @param  magnification = 1.0f	拡大率
 */
void EffectManager::Load(std::string filePath, std::string name, float magnification) {

	// 読み込み
	int res = LoadEffekseerEffect(filePath.c_str(), magnification);

	// リソースの管理
#if 0
	effectResourceMap[_filePath.c_str()] = res;
#else
	// 既に登録されているか、検索をする
	auto itr = effectResourceMap.find(filePath.c_str());
	if (itr == effectResourceMap.end()) {
		// 登録
		effectResourceMap.emplace(name.c_str(), res);
	}

#endif
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
		std::string filePath = eff["file"].get<std::string>();
		std::string name = eff["name"].get<std::string>();
		float magnification = eff.value("magnification", 1.0f);

		// エフェクトをすべてロード
		Load(filePath, name, magnification);
	}
}