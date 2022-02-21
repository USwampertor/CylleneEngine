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
    
    int
    createReport(void* exception) const;

    void
    createReport(const String& type, 
                 const String& description, 
                 const String& errorFunction, 
                 const String& file,
                 uint32 line = 0) const;

    static String
    getStackTrace();

    void
    logErrorAndStackTrace(const String& message, const String& stackTrace) const;

    void
    logErrorAndStackTrace(const String& type,
                          const String& strDescription,
                          const String& strFunction,
                          const String& strFile,
                          uint32 nLine) const;

    static const Path&
    getCrashFolder();

    void
    createDump();

    void
    openCrashHandlerApp();

    void
    shutdown();

  private:
    static const String m_crashFolder;
    static const String m_crashLog;
    static const String m_errorMessage;
#if CY_PLATFORM == CY_PLATFORM_WIN32
    struct Data;
    Data* m_data;
#endif
  };
}
