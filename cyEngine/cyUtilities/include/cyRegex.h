/**
 * @file cyRegex.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Regex.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"

#include <regex>

namespace CYLLENE_SDK
{
// Redefinitions
template<typename T, typename Tr = std::regex_traits<T>>
using BasicRegex = std::basic_regex<T, Tr>;
  
using Regex = std::regex;
  
using StringMatch = std::smatch;
  
using CharMatch = std::cmatch;

using SRegexIterator = std::sregex_iterator;

using CRegexIterator = std::cregex_iterator;

/**
 * @brief Makes a regex search with the given arguments
 * @return the object
 */
template <typename... Args>
decltype(auto) regexSearch(Args&&... args) {
  return std::regex_search(std::forward<Args>(args)...);
}

}

