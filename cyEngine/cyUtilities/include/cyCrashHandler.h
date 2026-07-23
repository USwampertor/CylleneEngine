/**
 * @file cyCrashHandler.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for CrashHandler.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyException.h"
#include "cyFileSystem.h"
#include "cyModule.h"

#define CY_MAX_STACKTRACE_DEPTH 200
#define CY_MAX_STACKTRACE_NAME_BYTES 1024

namespace CYLLENE_SDK
{
/**
 * @class CrashHandler
 * @brief Collects crash diagnostics and writes crash reports.
 */
class CY_UTILITY_EXPORT CrashHandler : public Module<CrashHandler>
{
 public:
  
  CrashHandler();

  ~CrashHandler();

  /**
   * @brief Initializes crash handling resources.
   */
  void
  init();

  /**
   * @brief Creates a report from platform exception data.
   * @param exception Platform exception pointer.
   * @return Status code of report generation.
   */
  int32
  createReport(void* exception) const;

  /**
   * @brief Creates a report from explicit crash metadata.
   * @param type Error type.
   * @param description Error description.
   * @param errorFunction Function where the error happened.
   * @param file Source file where the error happened.
   * @param line Source line number.
   */
  void
  createReport(Stringview type, 
               Stringview description, 
               Stringview errorFunction, 
               Stringview file,
               uint32 line = 0) const;

  /**
   * @brief Captures the current call stack.
   * @return Formatted stack trace.
   */
  static String
  getStackTrace();

  /**
   * @brief Logs an error message with a provided stack trace.
   * @param message Error message.
   * @param stackTrace Stack trace text.
   */
  void
  logErrorAndStackTrace(Stringview message, 
                        Stringview stackTrace) const;

  /**
   * @brief Logs fully detailed crash data and stack trace.
   * @param type Error type.
   * @param strDescription Error description.
   * @param strFunction Function name.
   * @param strFile Source file.
   * @param nLine Source line.
   */
  void
  logErrorAndStackTrace(Stringview type,
                        Stringview strDescription,
                        Stringview strFunction,
                        Stringview strFile,
                        uint32 nLine) const;

  /**
   * @brief Returns the folder used for crash artifacts.
   * @return Crash output folder path.
   */
  Path
  getCrashFolder();

  /**
   * @brief Creates a dump/report artifact.
   * @param message Crash message.
   * @param stackTrace Crash stack trace.
   * @return Generated dump path.
   */
  Path
  createDump(Stringview message,
             Stringview stackTrace);

  /**
   * @brief Opens the external crash handler application.
   * @param params Launch parameters.
   */
  void
  openCrashHandlerApp(Stringview params);

  /**
   * @brief Shuts down crash handling resources.
   */
  void
  shutdown();

private:

  /**
   * @brief Crash output folder name.
   */
  static const String m_crashFolder;

  /**
   * @brief Crash log file name.
   */
  static const String m_crashLog;

  /**
   * @brief Default crash message key.
   */
  static const String m_errorMessage;
#if CY_PLATFORM == CY_PLATFORM_WIN32
  /**
   * @brief Platform-specific crash handler state.
   */
  struct Data;

  /**
   * @brief Platform-specific crash handler data pointer.
   */
  Data* m_data;
#endif
};

}

