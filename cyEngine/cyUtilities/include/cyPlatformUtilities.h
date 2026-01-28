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

#include <type_traits>
#include <utility>

namespace CYLLENE_SDK {



template<typename T>
constexpr T&& forwarding(T&& arg) noexcept {
  return std::forward<T>(arg);
}

template <typename... Args>
constexpr auto forwardArgs(Args&&... args) noexcept {
  return std::forward_as_tuple(std::forward<Args>(args)...);
}

/*
 *	@struct PlatformUtils	
 *	@brief	A series of utilities that are agnostic to the OS
 *
 */
struct CY_UTILITY_EXPORT PlatformUtils {
public:

  /*
   *	@brief	runs a command with the given commandLine. This is a wrapper for system() function
   *	@param	const String& commandLine the cmd line to run
   */
  static void
  runCommand(const String& commandLine) {
    system(commandLine.c_str());
  }

  /*
   *	@brief	Formats a string with given parameters.
   *          This is a snprintf() wrapper but using String objects
   *	@param  const String& format the string with formats
   *	@param  Args ... args the arguments to pass to the formatting
   *  @return	A String with the parameters formatted
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

  /*
   *	@brief  formats the TM object into a specific string type	
   *	@param  const TM& toFormat the TM to format
   *	@param  const String& format the format 
   *  @return	String with the TM formatted
   */
  static String
  timeFormat(const TM& toformat, const String& format) {
    char buffer[128];
    std::strftime(buffer, sizeof(buffer), format.c_str(), &toformat);
    return String(buffer);
  }

  /*
   *	@brief Changes a number to a string. This is just a std::to_string wrapper
   *	@param  const T& number - The number to format to string
   *  @return	a String with the number
   */
  template<typename T>
  static String 
  toString(const T& number) {
    return std::to_string(number);
  }

  /*
   *	@brief	Throws a std::exception with the given message
   *	@param	const String& message the message the exception will have
   */
  static void 
  throwException(const String& message) {
    throw::std::exception(message.c_str());
  }

  /*
   *	@brief	throws a std::runtime_error with the given message
   *	@param	const String& message the message the runtime_error will have
   */
  static void
  throwRuntimeError(const String& message) {
    throw::std::runtime_error(message.c_str());
  }

  /*
   *	@brief	Converts a String to a Wide String
   *	@param	const String& str the string to transform
   *  @return	WString copy of the String
   */
  static WString
  toWide(const String& str) {
    WString stemp = WString(str.begin(), str.end());
    return stemp;

  }

  /*
   *	@brief  Converts a string to lowercase
   *	@param  const String& str the string to turn into lowercase
   *  @return	a copy of the string but in lowercase
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
  
  /*
   *	@brief  Converts a string to lowercase
   *	@param  const String& str the string to turn into lowercase
   *  @return	a copy of the string but in lowercase
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


  /*
   *	@brief  Creates a pair with given parameters. This is a wrapper for std::make_pair
   *	@param  T value1 value 1 from the pair
   *	@param  T value2 value 2 from the pair
   *  @return	a Pair<T, A> with the two given objects
   */
  template<typename T, typename A>
  static Pair<T, A>
  makePair(T value1, A value2) {
    return std::make_pair(value1, value2);
  }

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

  static bool
  isStringNumber(const String& s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
  }

  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>&
  endLine(std::basic_ostream<CharT, Traits>& os) {
    return std::endl(os);
  }


  /**
   * A blank string
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
