/*
 *	@file	EffectManager.h
 *  @author	oorui
 */

#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_
#include "../Singleton.h"
#include "../Engine.h"
#include "../JSON.h"
#include <map>

class EffectManager : public Singleton<EffectManager> {
	// フレンド宣言
	friend class Singleton<EffectManager>;

private:
	Engine* engine;										// ゲームエンジン参照
	std::map<std::string, int>effectResourceMap;		// リソースの連想配列
	std::list<class Effect*> pEffectList;				// エフェクトの一元配列

	JSON json;		// jsonデータ

private:
	EffectManager();
	~EffectManager() = default;

public:
	/*
	 *	エフェクトの読み込み
	 *  @param	filePath				ファイルパス
	 *  @param	name					エフェクトに設定する名前
	 *  @param  magnification = 1.0f	拡大率
	 */
	void Load(std::string filePath, std::string name, float magnification = 1.0f);

	/*
	 *	エフェクトを一括で読み込む
	 */
	void LoadEffects();


public:
	/*
	 *	jsonの変更
	 */
	void SetStageJSONData(JSON setJSON) { json = setJSON; }

};

#endif // !_EFFECTMANAGER_H_
