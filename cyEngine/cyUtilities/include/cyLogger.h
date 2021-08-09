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
  namespace LOG_VERBOSITY
  {
    BETTER_ENUM(E, uint32, 
      DEFAULT = 0, 
      DEBUG, 
      WARNING, 
      ERROR)
  }

  namespace LOG_CHANNEL
  {
    BETTER_ENUM(E, uint32,
      DEFAULT   = 0,
      ANIMATION ,
      AI        ,
      GRAPHICS  ,
      INPUT     ,
      SCRIPTING ,
      SOUND     ,
      SYSTEM    ,
      GAMEPLAY  
    );
  }

  class CY_UTILITY_EXPORT Log
  {
  public:
    Log() = default;

    ~Log() = default;

    Log(const String& message, 
        const LOG_VERBOSITY::E& type      = LOG_VERBOSITY::E::DEFAULT, 
        const LOG_CHANNEL::E& channel     = LOG_CHANNEL::E::DEFAULT,
        const TimePoint<SystemClock> time = SystemClock::now())
      : m_message(message), m_type(type), m_channel(channel), m_time(time) {}

    Log(const Log& otherLog) 
      : m_message(otherLog.m_message), 
        m_type(otherLog.m_type), 
        m_channel(otherLog.m_channel), 
        m_time(otherLog.m_time) 
      {}

    const String
    ToString();

    const String&
    GetMessage();

    const LOG_VERBOSITY::E&
    GetVerbosity();

    const LOG_CHANNEL::E&
    GetChannel();

  private:

    String                  m_message;
    LOG_VERBOSITY::E        m_type;
    LOG_CHANNEL::E          m_channel;
    TimePoint<SystemClock>  m_time;

  };


  class CY_UTILITY_EXPORT Logger : public Module<Logger>
  {
  public:

    void
    init();

    void
    clear();

    void
    log(Log newLog);

    void
    log(const String& message, 
        const LOG_VERBOSITY::E& type  = LOG_VERBOSITY::E::DEFAULT, 
        const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::DEFAULT);

    void
    logDebug(const String& message, const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::DEFAULT);

    void 
    logWarning(const String& message, const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::DEFAULT);

    void
    logError(const String& message, const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::DEFAULT);

    Vector<Log> m_logStack;

    void
    dump();

  };
}
