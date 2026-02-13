/*
 *	@file	FontManager.h
 *  @author oorui
 */

#ifndef FONTMANAGER_H_
#define FONTMANAGER_H_

#include <string>
#include <unordered_map>

#include "../VecMath.h"
#include "../Singleton.h"
 /*
  *	フォントのロード、使用をするマネージャー
  */
class FontManager : public Singleton<FontManager> {
	friend class Singleton<FontManager>;
private:
	// フォント用マップ
	std::unordered_map<std::string, int> m_fonts;
	// 使用するフォント種類
	const std::string m_fontType = "Noto Sans Thin";

public:
	FontManager() = default;
	~FontManager();

public:

	/*
	 *  @brief  フォントの読み込み
	 *  @param  int                 フォントのサイズ
	 *  @param  const std::string&  名前
	 */
	void LoadFont(int size, const std::string& name);

	/*
	 *  @brief  フォントの使用
	 *  @param  const std::string& 読み込み時に指定した名前
	 */
	void Draw(const std::string& name,
		int x,
		int y,
		const std::string& text,
		int color);

	/*
	 *  @brief  指定フォントの削除
	 *  @param  const std::string&  削除する名前
	 */
	void UnloadFont(const std::string& name);

	/*
	 *  全フォントの削除
	 */
	void ReleaseAll();
};

#endif // !FONTMANAGER_H_

