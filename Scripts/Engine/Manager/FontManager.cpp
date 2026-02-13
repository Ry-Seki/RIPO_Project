/*
 *	@file	FontManager.cpp
 *  @author oorui
 */
#include "FontManager.h"

/*
 *	デストラクタ
 */
FontManager::~FontManager() {
    ReleaseAll();
}


/*
 *  @brief  フォントの読み込み
 *  @param  int                 フォントのサイズ
 *  @param  const std::string&  名前
 */
void FontManager::LoadFont(int size, const std::string& name) {
    // 既に存在する場合は作成しない
    if (m_fonts.find(name) != m_fonts.end())
        return;

    // フォントハンドルを生成
    int handle = CreateFontToHandle(m_fontType.c_str(), size, -1);

    // フォントを登録
    if (handle != -1) {
        m_fonts[name] = handle;
    }
}

/*
 *  @brief  フォントの使用
 *  @param  const std::string& 読み込み時に指定した名前
 */
void FontManager::Draw(const std::string& name, int x, int y, const std::string& text, int color) {
    // フォントを検索
    auto it = m_fonts.find(name);

    // 未生成なら何もしない
    if (it == m_fonts.end())
        return; 

    // 描画
    DrawStringToHandle(x, y, text.c_str(), color, it->second);
}

/*
 *  @brief  指定フォントの削除
 *  @param  const std::string&  削除する名前
 */
void FontManager::UnloadFont(const std::string& name) {
    // 指定されたフォントを検索
    auto it = m_fonts.find(name);

    // 存在しない場合は何もしない
    if (it == m_fonts.end())
        return;

    // フォントハンドルを削除
    DeleteFontToHandle(it->second);

    // マップから削除
    m_fonts.erase(it);
}

/*
 *  全フォントの削除
 */
void FontManager::ReleaseAll() {
    for (auto& font : m_fonts) {
        // フォントハンドルを削除
        DeleteFontToHandle(font.second);
    }

    m_fonts.clear();
}

