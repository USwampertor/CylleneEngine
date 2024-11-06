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
#include "cyEvent.h"

namespace CYLLENE_SDK {

  namespace LOG_VERBOSITY
  {
    BETTER_ENUM(E, uint32,
                eDEFAULT = 0,
                eDEBUG,
                eWARNING,
                eERROR);
  }

  namespace LOG_CHANNEL
  {
  BETTER_ENUM(E, uint32,
              eDEFAULT = 0,
              eANIMATION,
              eAI,
              eGRAPHICS,
              eINPUT,
              eSCRIPTING,
              eSOUND,
              eSYSTEM,
              eGAMEPLAY);
  }

  namespace LOG_OUTPUT
  {
  BETTER_ENUM(E, uint32,
              eDEFAULT    = 1 << 0,
              eSPLASH     = 1 << 1,
              eDEBUGGER   = 1 << 2,
              eSCREEN     = 1 << 3,
              eCONSOLE    = 1 << 4);
  }

class CY_UTILITY_EXPORT Log
{
 public:

  ~Log() = default;

  Log(const String& message, 
      const LOG_VERBOSITY::E& type      = LOG_VERBOSITY::E::eDEFAULT, 
      const LOG_CHANNEL::E& channel     = LOG_CHANNEL::E::eDEFAULT,
      const Bitset<5>& output           = LOG_OUTPUT::E::eDEFAULT)
    : m_message(message), 
      m_type(type), 
      m_channel(channel), 
      m_output(output),
      m_time(SystemClock::now()) {}

  Log(const Log& otherLog) 
    : m_message(otherLog.m_message), 
      m_type(otherLog.m_type), 
      m_channel(otherLog.m_channel),
      m_output(otherLog.m_output),
      m_time(otherLog.m_time) {}

  const String
  ToString();

  const String&
  GetMessage();

  const LOG_VERBOSITY::E&
  GetVerbosity();

  const LOG_CHANNEL::E&
  GetChannel();

  const LOG_OUTPUT::E&
  GetOutput();
  
private:

  String                  m_message;
  
  LOG_VERBOSITY::E        m_type;
  
  LOG_CHANNEL::E          m_channel;

  Bitset<5>               m_output;

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
      const LOG_VERBOSITY::E& type  = LOG_VERBOSITY::E::eDEFAULT, 
      const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT,
      const Bitset<5>& output       = LOG_OUTPUT::E::eDEFAULT);

  void
  logDebug(const String& message, 
           const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT,
           const Bitset<5>& output       = LOG_OUTPUT::E::eDEFAULT);

  void 
  logWarning(const String& message, 
             const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT,
             const Bitset<5>& output       = LOG_OUTPUT::E::eDEFAULT);

  void
  logError(const String& message, 
           const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT,
           const Bitset<5>& output       = LOG_OUTPUT::E::eDEFAULT);

  void 
  toConsole(const String& message,
            const LOG_VERBOSITY::E& type = LOG_VERBOSITY::E::eDEFAULT,
            const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT);

  void 
  toSplash(const String& message,
           const LOG_VERBOSITY::E& type = LOG_VERBOSITY::E::eDEFAULT,
           const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT);

  void 
  toDebugger(const String& message,
             const LOG_VERBOSITY::E& type = LOG_VERBOSITY::E::eDEFAULT,
             const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT);

  void 
  toScreen(const String& message,
           const LOG_VERBOSITY::E& type = LOG_VERBOSITY::E::eDEFAULT,
           const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT);

  void
  dump();

  Event<void, const Log&>& onLogAdded() { return m_onLogAdded; }

private:
  Vector<Log> m_logStack;

  Event<void, const Log&> m_onLogAdded;

};

#define CY_LOG(message, type, channel, ...)                                           \
        Logger::instance().log((Utils::format(message, ##__VA_ARGS__)+                \
                               String("\n\t\t in ") +                                 \
                               __PRETTY_FUNCTION__ +                                  \
                               " [" + __FILE__ + ":" +                                \
                               Utils::toString(__LINE__) + "]\n"), type, channel);
}
