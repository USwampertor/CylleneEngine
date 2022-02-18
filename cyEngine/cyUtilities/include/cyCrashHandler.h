/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyCrashHandler.h
 * @author Marco "Swampy" Millan
 * @date 8/6/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyModule.h"
#include "cyUtilitiesPrerequisites.h"
#include "cyFileSystem.h"

#define CY_MAX_STACKTRACE_DEPTH 200
#define CY_MAX_STACKTRACE_NAME_BYTES 1024

namespace CYLLENE_SDK
{
  class CY_UTILITY_EXPORT CrashHandler : public Module<CrashHandler>
  {
   public:
    
    CrashHandler() = default;

    ~CrashHandler() = default;

    void
    init();

    void 
    createReport(StdException exception);
    
    void
    createReport(void* exception) const;

    void
    createReport(const String& type, 
                 const String& description, 
                 const String& errorFunction, 
                 const String& file,
                 uint32 line) const;

    String
    getPlatformStack();

    String
    getStack();

    void
    logErrorAndStackTrace(const String& message, const String stackTrace) const;

    void
    logErrorAndStackTrace(const String& type,
                          const String& strDescription,
                          const String& strFunction,
                          const String& strFile,
                          uint32 nLine) const;

    void
    showCallStack();

    void*
    loadModuleSymbols(void* process, String pid);

    Path
    getCrashFolder();

    void
    createMiniDump();

    void
    openCrashHandlerApp();

    void
    shutdown();

  private:
    String m_crashFolder;
    String m_crashLog;
    String m_errorMessage;

    void* m_data;
  };
}
