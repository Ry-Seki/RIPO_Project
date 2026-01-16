/*
 *	@file	StringUtility.h
 *	@author	Seki
 */

#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include <string>
#include <vector>

namespace StringUtility {
    /*
     *  @brief      •¶š—ñ‚ğØ‚è•ª‚¯‚é
     *  @param[in]  const std::string& splitString      Ø‚èæ‚è‘ÎÛ‚Ì•¶š—ñ
     *  @param[in]  char delimiter                      Ø‚èæ‚é‹«ŠE‚Æ‚È‚é•¶š
     *  @return     std::vector<std::string>
     */
    std::vector<std::string> Split(const std::string& divideString, char delimiter);
}

#endif // !_STRING_UTILITY_H_