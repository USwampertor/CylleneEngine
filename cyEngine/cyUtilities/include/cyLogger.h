/**
 * @file cyLogger.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Logger.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyModule.h"
#include "cyEvent.h"
#include "cyChronos.h"

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

/**
 * @class Log
 * @brief Immutable log entry with message, category, and output targets.
 */
class CY_UTILITY_EXPORT Log
{
 public:

  ~Log() = default;

  Log(Stringview message, 
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

  /**
   * @brief Converts log entry to string.
   * @return Formatted log string.
   */
  const String
  toString() const;

  /**
   * @brief Gets stored message text.
   * @return Message text.
   */
  Stringview
  getMsg() const;

  /**
   * @brief Gets message verbosity.
   * @return Verbosity level.
   */
  const LOG_VERBOSITY::E&
  getVerbosity() const;

  /**
   * @brief Gets message channel.
   * @return Channel enum.
   */
  const LOG_CHANNEL::E&
  getChannel() const;

  /**
   * @brief Gets output destinations.
   * @return Output bitset.
   */
  const Bitset<5>&
  getOutput() const;
  
private:

  /**
   * @brief Log text message.
   */
  String                  m_message;
  
  /**
   * @brief Log verbosity.
   */
  LOG_VERBOSITY::E        m_type;
  
  /**
   * @brief Log channel.
   */
  LOG_CHANNEL::E          m_channel;

  /**
   * @brief Output destinations bitset.
   */
  Bitset<5>               m_output;

  /**
   * @brief Log timestamp.
   */
  TimePoint<SystemClock>  m_time;

};

/**
 * @class Logger
 * @brief Central logging module for formatted engine output.
 */
class CY_UTILITY_EXPORT Logger : public Module<Logger>
{
public:

  virtual void
  onStartUp() override;

  /**
   * @brief Initializes logger internals and outputs.
   */
  void
  init();

  /**
   * @brief Clears stored log history.
   */
  void
  clear();

  /**
   * @brief Adds a prepared log entry.
   * @param newLog Log entry to append.
   */
  void
  log(Log newLog);

  /**
   * @brief Logs a formatted message.
   * @param message Message text.
   * @param type Verbosity level.
   * @param channel Log channel.
   * @param output Output destinations.
   */
  void
  log(Stringview message, 
      const LOG_VERBOSITY::E& type  = LOG_VERBOSITY::E::eDEFAULT, 
      const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT,
      const Bitset<5>& output       = LOG_OUTPUT::E::eDEFAULT);

  /**
   * @brief Logs a debug message.
   * @param message Message text.
   * @param channel Log channel.
   * @param output Output destinations.
   */
  void
  logDebug(Stringview message, 
           const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT,
           const Bitset<5>& output       = LOG_OUTPUT::E::eDEFAULT);

  /**
   * @brief Logs a warning message.
   * @param message Message text.
   * @param channel Log channel.
   * @param output Output destinations.
   */
  void 
  logWarning(Stringview message, 
             const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT,
             const Bitset<5>& output       = LOG_OUTPUT::E::eDEFAULT);

  /**
   * @brief Logs an error message.
   * @param message Message text.
   * @param channel Log channel.
   * @param output Output destinations.
   */
  void
  logError(Stringview message, 
           const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT,
           const Bitset<5>& output       = LOG_OUTPUT::E::eDEFAULT);

  /**
   * @brief Sends a log line to console output.
   * @param message Message text.
   * @param type Verbosity level.
   * @param channel Log channel.
   */
  void 
  toConsole(Stringview message,
            const LOG_VERBOSITY::E& type = LOG_VERBOSITY::E::eDEFAULT,
            const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT);

  /**
   * @brief Sends a log line to splash output.
   * @param message Message text.
   * @param type Verbosity level.
   * @param channel Log channel.
   */
  void 
  toSplash(Stringview message,
           const LOG_VERBOSITY::E& type = LOG_VERBOSITY::E::eDEFAULT,
           const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT);

  /**
   * @brief Sends a log line to debugger output.
   * @param message Message text.
   * @param type Verbosity level.
   * @param channel Log channel.
   */
  void 
  toDebugger(Stringview message,
             const LOG_VERBOSITY::E& type = LOG_VERBOSITY::E::eDEFAULT,
             const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT);

  /**
   * @brief Sends a log line to screen output.
   * @param message Message text.
   * @param type Verbosity level.
   * @param channel Log channel.
   */
  void 
  toScreen(Stringview message,
           const LOG_VERBOSITY::E& type = LOG_VERBOSITY::E::eDEFAULT,
           const LOG_CHANNEL::E& channel = LOG_CHANNEL::E::eDEFAULT);

  /**
   * @brief Dumps accumulated logs to persistent output.
   */
  void
  dump();

  /**
   * @brief Accesses log-added event.
   * @return Event fired for each appended log.
   */
  Event<void, const Log&>& onLogAdded() { return m_onLogAdded; }

private:
  /**
   * @brief Stored log entries.
   */
  Vector<Log> m_logStack;

  /**
   * @brief Event fired when a log is added.
   */
  Event<void, const Log&> m_onLogAdded;

};

#define CY_LOG(message, type, channel, ...)                                           \
        Logger::instance().log((Utils::format(message, ##__VA_ARGS__)+                \
                               String("\n\t\t in ") +                                 \
                               __PRETTY_FUNCTION__ +                                  \
                               " [" + __FILE__ + ":" +                                \
                               Utils::toString(__LINE__) + "]\n"), type, channel);
}

