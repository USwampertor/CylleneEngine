/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file Utilities.h
 * @author Marco "Swampy" Millan
 * @date 8/6/2021
 * @brief
 *
 */
 /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK {
struct CY_UTILITY_EXPORT PlatformUtils {
public:

  static void
  runCommand(const String& commandLine) {
    system(commandLine.c_str());
  }

  template<typename ... Args>
  static String format(const String& format, Args ... args) {
    int32 size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throwRuntimeError("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return String(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
  }


  static String
  timeFormat(const TM& toformat, const String& format) {
    char buffer[128];
    std::strftime(buffer, sizeof(buffer), format.c_str(), &toformat);
    return String(buffer);
  }

  template<typename T>
  static String 
  toString(const T& number) {
    return std::to_string(number);
  }

  static void 
  throwException(const String& message) {
    throw::std::exception(message.c_str());
  }

  static void 
  throwRuntimeError(const String& message) {
    throw::std::runtime_error(message.c_str());
  }

  static const String BLANK;

  static WString
  toWide(String str) {
    WString stemp = WString(str.begin(), str.end());
    return stemp;

  }

  template<typename T, typename A>
  static Pair<T, A>
  makePair(T value1, A value2) {
    return std::make_pair(value1, value2);
  }


};
}
