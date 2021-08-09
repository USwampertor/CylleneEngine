/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyLogger.cpp
 * @author Marco "Swampy" Millan
 * @date 8/6/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#include "cyLogger.h"
#include "cyUtilities.h"

namespace CYLLENE_SDK {
  const String&
  Log::GetMessage() {
    return m_message;
  }

  const LOG_VERBOSITY::E&
  Log::GetVerbosity() {
    return m_type;
  }

  const LOG_CHANNEL::E&
  Log::GetChannel() {
    return m_channel;
  }

  const String
  Log::ToString() {
    String toReturn = "";
    TimeType tt = SystemClock::to_time_t(m_time);
    TM localTime = *localtime(&tt);
    
    toReturn += Utilities::format("[%s] [%s %s] %s",  
                                  Utilities::timeFormat(localTime, "%H:%M:%S").c_str(),
                                  m_type._to_string(),
                                  m_channel._to_string(),
                                  m_message.c_str());
    return toReturn;
  }

  void
  Logger::init() {
    m_logStack.clear();
    log("Initialized Logger Module");
  }

  void 
  Logger::clear() {
    m_logStack.clear();
  }

  void 
  Logger::log(Log newLog) {
    m_logStack.push_back(newLog);
  }

  void 
  Logger::log(const String& message, 
              const LOG_VERBOSITY::E& type, 
              const LOG_CHANNEL::E& channel) {
    m_logStack.push_back(Log(message, type, channel));
  }

  void
  Logger::logDebug(const String& message, const LOG_CHANNEL::E& channel) {
    m_logStack.push_back(Log(message, LOG_VERBOSITY::E::DEBUG, channel));
  }

  void
  Logger::logWarning(const String& message, const LOG_CHANNEL::E& channel) {
    m_logStack.push_back(Log(message, LOG_VERBOSITY::E::WARNING, channel));
  }

  void
  Logger::logError(const String& message, const LOG_CHANNEL::E& channel) {
    m_logStack.push_back(Log(message, LOG_VERBOSITY::E::ERROR, channel));
  }

  void
  Logger::dump()
  {
    
  }

}