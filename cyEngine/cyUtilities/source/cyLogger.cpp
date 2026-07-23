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
#include "cyTime.h"


namespace CYLLENE_SDK {
  Stringview
  Log::getMsg() const {
    return m_message;
  }

  const LOG_VERBOSITY::E&
  Log::getVerbosity() const {
    return m_type;
  }

  const LOG_CHANNEL::E&
  Log::getChannel() const {
    return m_channel;
  }

  const String
  Log::toString() const {
    String toReturn = "";
    Date localTime = Time::scToDate(m_time);
    
    toReturn += Utils::format("[%s] [%s %s] %s",  
                                  Utils::timeFormat(localTime, "%H:%M:%S").c_str(),
                                  m_type._to_string(),
                                  m_channel._to_string(),
                                  m_message.c_str());
    return toReturn;
  }

  const Bitset<5>&
  Log::getOutput() const {
    return m_output;
  }

  void 
  Logger::onStartUp() {
    Logger::instance().init();
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
    m_onLogAdded.invoke(newLog);
  }

  void 
  Logger::log(Stringview message, 
              const LOG_VERBOSITY::E& type, 
              const LOG_CHANNEL::E& channel,
              const Bitset<5>& output) {
    Log newLog(message, type, channel, output);
    m_logStack.push_back(newLog);
    m_onLogAdded.invoke(newLog);
  }

  void
  Logger::logDebug(Stringview message, 
                   const LOG_CHANNEL::E& channel,
                   const Bitset<5>& output) {
    Log newLog(message, LOG_VERBOSITY::E::eDEBUG, channel, output);
    m_logStack.push_back(newLog);
    m_onLogAdded.invoke(newLog);
  }

  void
  Logger::logWarning(Stringview message, 
                     const LOG_CHANNEL::E& channel,
                     const Bitset<5>& output) {
    Log newLog(message, LOG_VERBOSITY::E::eWARNING, channel, output);
    m_logStack.push_back(newLog);
    m_onLogAdded.invoke(newLog);
  }

  void
  Logger::logError(Stringview message, 
                   const LOG_CHANNEL::E& channel,
                   const Bitset<5>& output) {
    Log newLog(message, LOG_VERBOSITY::E::eERROR, channel, output);
    m_logStack.push_back(newLog);
    m_onLogAdded.invoke(newLog);
  }

  void
  Logger::dump() {
    Path logPath = FileSystem::getExecutablePath().directoryPath() + "/logs/";
    if (!FileSystem::exists(logPath.path())) {
      FileSystem::createFolder(logPath.path());
    }
    logPath = logPath.path() + Utils::format("log_%s.txt", Utils::timeFormat(Time::now(), "%Y%m%d_%H%M%S").c_str());

    File logFile = FileSystem::createFile(logPath.path());

    String data = "----- Log Dump -----\n";
    for (const Log& logEntry : m_logStack) {
      data += logEntry.toString() + "\n";
    }
    logFile.writeFile(data);
  }

}