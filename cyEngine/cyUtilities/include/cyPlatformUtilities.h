/**
 * @file cyPlatformUtilities.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for PlatformUtilities.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"

#include <type_traits>
#include <utility>

namespace CYLLENE_SDK {

/**
 * @brief Perfect-forwards an argument.
 * @tparam T Argument type.
 * @param arg Argument to forward.
 * @return Forwarded argument.
 */
template<typename T>
constexpr T&& forwarding(T&& arg) noexcept {
  return std::forward<T>(arg);
}

/**
 * @brief Packs forwarded arguments into a tuple.
 * @tparam Args Argument types.
 * @param args Arguments to forward.
 * @return Tuple of forwarded arguments.
 */
template <typename... Args>
constexpr auto forwardArgs(Args&&... args) noexcept {
  return std::forward_as_tuple(std::forward<Args>(args)...);
}

/**
 * @struct PlatformUtils
 * @brief OS-agnostic utility helpers for strings, formatting, and process control.
 */
struct CY_UTILITY_EXPORT PlatformUtils {
public:

  /**
   * @brief Runs a shell command line.
   * @param commandLine Command to execute.
   */
  static void
  runCommand(const String& commandLine) {
    system(commandLine.c_str());
  }

  /**
   * @brief Formats a string using printf-style placeholders.
   * @tparam Args Argument types.
   * @param formatStr Format string.
   * @param args Format arguments.
   * @return Formatted string.
   */
  template<typename ... Args>
  static String format(const String& formatStr, Args ... args) {

    const char* formatCstr = formatStr.c_str();

    int32 size_s = std::snprintf(nullptr, 
                                 0, 
                                 formatCstr,
                                 std::forward<Args>(args)...) + 1;
                                 // std::forward<Args>(args)...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throwRuntimeError("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, formatCstr, std::forward<Args>(args)...);
    return String(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
  }

  /**
   * @brief Formats a `tm` value using a strftime format string.
   * @param toformat Time structure to format.
   * @param format Strftime format string.
   * @return Formatted time string.
   */
  static String
  timeFormat(const TM& toformat, const String& format) {
    char buffer[128];
    std::strftime(buffer, sizeof(buffer), format.c_str(), &toformat);
    return String(buffer);
  }

  /**
   * @brief Converts a numeric value to string.
   * @tparam T Numeric type.
   * @param number Number to convert.
   * @return Converted string.
   */
  template<typename T>
  static String 
  toString(const T& number) {
    return std::to_string(number);
  }

  /**
   * @brief Throws a standard exception with the given message.
   * @param message Exception message.
   */
  static void 
  throwException(const String& message) {
    throw::std::exception(message.c_str());
  }

  /**
   * @brief Throws a runtime error with the given message.
   * @param message Exception message.
   */
  static void
  throwRuntimeError(const String& message) {
    throw::std::runtime_error(message.c_str());
  }

  /**
   * @brief Converts UTF-8-ish narrow string to wide string.
   * @param str Input narrow string.
   * @return Converted wide string.
   */
  static WString
  toWide(const String& str) {
    WString stemp = WString(str.begin(), str.end());
    return stemp;

  }

  /**
   * @brief Converts a string to lowercase.
   * @param str Input string.
   * @return Lowercased copy.
   */
  static String
  toLowerCase(const String& str) {
    String copy = str;
    std::transform(copy.begin(), 
                   copy.end(), 
                   copy.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return copy;
  }
  
  /**
   * @brief Converts a string to uppercase.
   * @param str Input string.
   * @return Uppercased copy.
   */
  static String
  toUpperCase(const String& str) {
    String copy = str;
    std::transform(copy.begin(), 
                   copy.end(), 
                   copy.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return copy;
  }

  /**
   * @brief Creates a pair from two values.
   * @tparam T First value type.
   * @tparam A Second value type.
   * @param value1 First value.
   * @param value2 Second value.
   * @return Pair containing both values.
   */
  template<typename T, typename A>
  static Pair<T, A>
  makePair(T value1, A value2) {
    return std::make_pair(value1, value2);
  }

  /**
   * @brief Converts an integer to hexadecimal string.
   * @param toValue Integer value to convert.
   * @param optionalPrefix If true, prepend `0x`.
   * @param toFillWith Optional padding character.
   * @return Uppercase hexadecimal string.
   */
  static String 
  intToHex(const int32& toValue, bool optionalPrefix = false, char toFillWith = '\0') {
    StringStream stream;
    stream << std::hex
      << std::uppercase // Convert to hex and uppercase
      << toValue;
    String prefix = (optionalPrefix ? "0x" : "");
    if (toFillWith != '\0')
    {
      SizeT size = static_cast<SizeT>(6 - stream.str().size());
      String middle(size, toFillWith);
      return prefix + middle + stream.str();
    }
    return prefix + stream.str();
  }

  /**
   * @brief Checks whether a string contains only decimal digits.
   * @param s Input string.
   * @return True if the string is a valid non-empty number.
   */
  static bool
  isStringNumber(const String& s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
  }

  /**
   * @brief Stream manipulator wrapper that inserts `std::endl`.
   * @tparam CharT Character type.
   * @tparam Traits Stream traits.
   * @param os Output stream.
   * @return Output stream reference.
   */
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>&
  endLine(std::basic_ostream<CharT, Traits>& os) {
    return std::endl(os);
  }

  /**
   * @brief Shared blank string constant.
   */
  static const String BLANKSTRING;
};

// 
// // helper that replicates std::forward semantics (safe to call as forwardArg<Args>(args)...)
// template<typename T>
// constexpr T&& forwardArg(std::remove_reference_t<T>& arg) noexcept {
//   return static_cast<T&&>(arg);
// }
// 
// template<typename T>
// constexpr T&& forwardArg(std::remove_reference_t<T>&& arg) noexcept {
//   static_assert(!std::is_lvalue_reference<T>::value, "bad forwardArg<T> call");
//   return static_cast<T&&>(arg);
// }

}

