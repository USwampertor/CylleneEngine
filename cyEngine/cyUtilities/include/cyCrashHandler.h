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
    createReport(Exception exception);
    
    void
    createReport(void* exception);

    void
    createReport(const String& type, 
                 const String& description, 
                 const String& errorFunction, 
                 const String& file);

    String
    getStack();

    Path
    getCrashFolder();

    void
    createMiniDump();

    void
    openCrashHandler();

    void
    shutdown();

  private:
    String m_crashFolder;
    String m_crashLog;
    String m_errorMessage;

  };
}
