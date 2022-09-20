// /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
// /**
//  * @file cyOmniverse.h
//  * @author Marco "Swampy" Millan
//  * @date 06/09/2022
//  * @brief 
//  * 
//  */
// /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
// #pragma once
// 
#include "cyOmniversePrerequisites.h"

#include <cyModule.h>
#include <cyLogger.h>
#include <cyFileSystem.h>
// 

namespace CYLLENE_SDK
{

namespace OMNILIVEMODE
{
  BETTER_ENUM(E, uint32,
    eDEFAULT = 0,
    eDISABLED,
    eENABLED);
}

namespace OMNILOGLEVEL
{
  BETTER_ENUM(E, uint32, 
              eDEBUG, // Extra chatty
              eVERBOSE, // Chatty
              eINFO, // Not a problem
              eWARNING, // Potential problem
              eERROR); // Definite problem)
}

namespace OMNIFILESTATUS
{
  BETTER_ENUM(E, uint32, 
    eFILESTATUSREADING,
    eFILESTATUSWRITING,
    eFILESTATUSCOPYING,
    eFILESTATUSDELETING,
    eLIVEUPDATESENDING,
    eLIVEUPDATERECEIVED);
}

namespace OMNICONNECTIONSTATUS
{
  BETTER_ENUM(E, uint32,
    eCONNECTIONSTATUSCONNECTING,
    eCONNECTIONSTATUSCONNECTED,
    eCONNECTIONSTATUSCONNECTERROR,
    eCONNECTIONSTATUSDISCONNECT);
}


class CY_OMNIVERSE_EXPORT Omniverse : public Module<Omniverse> {
public:
  
  static void
  omniClientCallback(void* userData,
                     const char* url,
                     OmniClientConnectionStatus status) noexcept {
    if (!Omniverse::isStartedUp()) {
      Logger::instance().logError(
        String("Omniverse Module was not started! This function should be called after"), 
        LOG_CHANNEL::E::eSYSTEM);
      return;
    }

    // Let's just print this regardless
    {
      UniqueLock<Mutex> lk(Omniverse::instance().m_logMutex);

      String connectionStatus = Utils::format("Connection Status: %s [%s]", 
                                              omniClientGetConnectionStatusString(status), 
                                              url);
      std::cout << connectionStatus << std::endl;
      Logger::instance().logDebug(connectionStatus, LOG_CHANNEL::E::eSYSTEM);
    }
    if (status == eOmniClientConnectionStatus_ConnectError) {
      std::string error("[ERROR] Failed connection, exiting.");
      std::cout << error << std::endl;
      Logger::instance().logDebug(error, LOG_CHANNEL::E::eSYSTEM);

    }
  }

  static void
  copyCallback() {

  }
  
  static void
  failNotify(const char* msg, const char* detail = nullptr, ...) {
    if (!Omniverse::isStartedUp()) {
      Logger::instance().logError(
        String("Omniverse Module was not started! This function should be called after"),
        LOG_CHANNEL::E::eSYSTEM);
      return;
    }

    UniqueLock<Mutex> lk(Omniverse::instance().m_logMutex);

    Logger::instance().logError(msg, LOG_CHANNEL::E::eSYSTEM);
    fprintf(stderr, "%s\n", msg);
    if (detail != nullptr) {
      Logger::instance().logError(detail, LOG_CHANNEL::E::eSYSTEM);
      fprintf(stderr, "%s\n", detail);
    }
  }
  
  // Omniverse Log callback
  static void
  logCallback(const char* threadName,
              const char* component,
              OmniClientLogLevel level,
              const char* message) noexcept {
    UniqueLock<Mutex> lk(Omniverse::instance().m_logMutex);
    if (Omniverse::instance().m_omniverseLogEnabled) {
      Logger::instance().logError(message, LOG_CHANNEL::E::eSYSTEM);
      printf(message);
      Omniverse::instance().m_message = message;
    }
  }


  bool
  init(bool doLiveEdit, const OMNILOGLEVEL::E& omniverseLog);

  virtual void
  onShutDown() override;

  void
  clear();

  String
  openUSD(const String& fileName);

  String
  createUSD(const String& fileName);

  UsdStageRefPtr
  getActiveStage() {
    return m_stage;
  }

  bool
  deleteUSD(const String& fileName);

  void
  closeUSD();

  void
  saveUSD();

  void
  UpdateUSD();

  /*
   *	@brief  This function translates the information from the Cyllene Engine Scenegraph system 
   *          to the Omniverse USD system.
   *	@param  const String& fileName the name of the file, in case the file is not existent, it will create 
   *          a new one
   *	@return -
   */
  void
  saveToUSD(const String& fileName);

  void
  checkpointFile(const String& url, const String& comment);

  String
  getActiveUsername(const String& stageUrl);

  bool
  isValidOmniURL(const String& filePath);

  File
  downloadFile(const String& filePath);

  bool
  uploadFile(const String& filePath, const String& destiny);

  File
  downloadMaterial(const String& materialPath);

  bool
  uploadMaterial(const String& materialPath, const String& destiny);

  bool
  isDefaultLiveSyncEnabled();

  const OMNILIVEMODE::E&
  isStageLiveSyncEnabled(const String& url);

  bool
  isOVLogEnabled();

  void
  setDefaultLiveSync(bool newStatus);

  void
  setStageLiveSync(const String& url, const OMNILIVEMODE::E& newStatus);

  void
  setOVLog(const OMNILOGLEVEL::E& logLevel);

  void
  waitForUpdates();

  String
  getVersion();

  const String&
  getLog();

private:
  // bool            m_doLiveEdit = true;
  bool            m_omniverseLogEnabled = false;
  String          m_message;
  UsdStageRefPtr  m_stage;
  std::mutex      m_logMutex;

};
}
