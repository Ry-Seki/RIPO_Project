/*
 *	@file	StringUtility.cpp
 *	@author	Seki
 */

#include "StringUtility.h"

namespace StringUtility {
    /*
	 *  @brief      •¶š—ñ‚ğØ‚è•ª‚¯‚é
	 *  @param[in]  const std::string& splitString      Ø‚èæ‚è‘ÎÛ‚Ì•¶š—ñ
	 *  @param[in]  char delimiter                      Ø‚èæ‚é‹«ŠE‚Æ‚È‚é•¶š
	 *  @return     std::vector<std::string>
	 */
    std::vector<std::string> Split(const std::string& divideString, char delimiter) {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t pos = 0;

        // Ø‚èæ‚ê‚È‚­‚È‚é‚Ü‚ÅŒJ‚è•Ô‚·
        while ((pos = divideString.find(delimiter, start)) != std::string::npos) {
            // •¶š—ñ‚ğØ‚èo‚µ‚Ä’Ç‰Á
            tokens.emplace_back(divideString.substr(start, pos - start));
            start = pos + 1;
        }
        // ÅŒã‚Ì—v‘f‚ğ‘¦’Ç‰Á‚·‚é
        tokens.emplace_back(divideString.substr(start));

        return tokens;
	}
}
