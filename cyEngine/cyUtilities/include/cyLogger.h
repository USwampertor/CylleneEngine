/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyLogger.h
 * @author Marco "Swampy" Millan
 * @date 8/4/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyModule.h"

namespace CYLLENE_SDK {
  namespace LOG_TYPE
  {
    enum E : uint32
    {
      DEFAULT = 0,
      DEBUG,
      WARNING,
      ERROR,
      SUCCESS
    };
  }

  class CY_UTILITY_EXPORT Log
  {
  public:
    Log() = default;

    Log(const String& message, const LOG_TYPE::E type = LOG_TYPE::DEFAULT) 
      : m_message(message), m_type(type) {}

    Log(const Log& otherLog) 
      : m_message(otherLog.m_message), m_type(otherLog.m_type) {}

    

  private:

    String m_message;
    LOG_TYPE::E m_type;

  };

  class CY_UTILITY_EXPORT Logger : public Module<Logger>
  {

  };

}
