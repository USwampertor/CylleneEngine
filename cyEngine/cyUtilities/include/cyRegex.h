/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	cyRegex.h
 * @author 	Marco "Swampy" Millan
 * @date 	2024/11/12
 * @brief 	
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyUtilitiesPrerequisites.h"

#include <regex>

namespace CYLLENE_SDK
{
  template<typename T, typename Tr = std::regex_traits<T>>
  using BasicRegex = std::basic_regex<T, Tr>;
  using Regex = std::regex;
  using StringMatch = std::smatch;
  using CharMatch = std::cmatch;
}

