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
  struct CY_UTILITY_EXPORT Utils {
  public:
    
    /**
     * @brief 
     * @param 
     * @return 
     *
     */
    template<typename ... Args>
    static String format(const String& format, Args ... args)
    {
      int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
      if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
      auto size = static_cast<size_t>(size_s);
      auto buf = std::make_unique<char[]>(size);
      std::snprintf(buf.get(), size, format.c_str(), args ...);
      return String(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }


    static String timeFormat(const TM& toformat, const String& format)
    {
      char buffer[128];
      std::strftime(buffer, sizeof(buffer), format.c_str(), &toformat);
      return String(buffer);
    }

    template<typename T>
    static 
    String toString(const T& number) {
      return std::to_string(number);
    }

    static 
    void ThrowException(const String& message) {
      throw::std::exception(message.c_str());
    }

  };
}
