/*
 *	@file	LoadSprite.h
 *	@author	Seki
 */

#ifndef _LOAD_GRAPH_H_
#define _LOAD_GRAPH_H_

#include "../LoadBase.h"
#include <DxLib.h>

/*
 *	DxLibの画像読み込みクラス
 */
class LoadSprite : public LoadBase {
private:
	int graphHandle = -1;	// 画像ハンドル

public:
	/*
     *	コンストラクタ
     *	param[in]	const std::string& setFilePath		ファイルパス
     */
    explicit LoadSprite(const std::string& setFilePath) : LoadBase(setFilePath) {}
    /*
     *  デストラクタ
     */
    ~LoadSprite() override { DeleteGraph(graphHandle); }

public:
    /*
     *  読み込み処理
     */
    void Load() override;

public:
    /*
     *  画像ハンドルの取得
     *  return  int
     */
    inline int GetHandle() const { return graphHandle; }

};

#endif // !_LOAD_GRAPH_H_
