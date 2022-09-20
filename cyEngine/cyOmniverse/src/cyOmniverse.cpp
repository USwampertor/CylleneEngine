#include "cyOmniverse.h"

namespace CYLLENE_SDK {
  
  bool
  Omniverse::init(bool doLiveEdit, const OMNILOGLEVEL::E& omniverseLog) {
    // Register a function to be called whenever the library wants to print something to a log
    omniClientSetLogCallback(logCallback);

    // The default log level is "Info", set it to "Debug" to see all messages
    setOVLog(omniverseLog);

    // Initialize the library and pass it the version constant defined in OmniClient.h
    // This allows the library to verify it was built with a compatible version. It will
    // return false if there is a version mismatch.
    if (!omniClientInitialize(kOmniClientVersion)) {
      return false;
    }

    omniClientRegisterConnectionStatusCallback(nullptr, Omniverse::omniClientCallback);

    // Enable live updates
    setDefaultLiveSync(doLiveEdit);

    return true;
  }

  void
  Omniverse::onShutDown() {
    // Calling this prior to shutdown ensures that all pending live updates complete.
    waitForUpdates();

    // The stage is a sophisticated object that needs to be destroyed properly.  
    // Since gStage is a smart pointer we can just reset it
    m_stage.Reset();

    //omniClientTick(1000);
    omniClientShutdown();
  }

  void
  Omniverse::clear() {
    m_stage.Reset();
  }

  String
  Omniverse::openUSD(const String& fileName) {
    waitForUpdates();

    clear();

    if (!isValidOmniURL(fileName)) {
      String error = Utils::format("%s is not a valid Omniverse URL", fileName.c_str());
      Logger::instance().logError(error, LOG_CHANNEL::E::eSYSTEM);
      return String("");
    }

    m_stage = UsdStage::Open(fileName);

    if (!m_stage) {
      String error = Utils::format("Failure to open Omniverse stage at: %s", fileName.c_str());
      Omniverse::failNotify(error.c_str(), fileName.c_str());
      Logger::instance().logError(error, LOG_CHANNEL::E::eSYSTEM);
      return String("");
    }

    {
      std::unique_lock<std::mutex> lk(m_logMutex);
      String message = Utils::format("New stage loaded: %s", fileName.c_str());
      std::cout << message << std::endl;
      Logger::instance().logDebug(message, LOG_CHANNEL::E::eSYSTEM);
    }

    // Always a good idea to declare your up-ness
    UsdGeomSetStageUpAxis(m_stage, UsdGeomTokens->y);

    return fileName;
  }

  String
  Omniverse::createUSD(const String& fileName) {
    if (!deleteUSD(fileName)) {
      String error = Utils::format("Failure to delete Omniverse stage at: %s", fileName.c_str());
      Omniverse::failNotify(error.c_str(), fileName.c_str());
      Logger::instance().logError(error, LOG_CHANNEL::E::eSYSTEM);
      return String("");
    }

    m_stage = UsdStage::CreateNew(fileName);
    if (!m_stage)
    {
      String error = Utils::format("Failure to create Omniverse stage at: %s", fileName.c_str());
      Omniverse::failNotify(error.c_str(), fileName.c_str());
      Logger::instance().logError(error, LOG_CHANNEL::E::eSYSTEM);
      return String("");
    }

    {
      std::unique_lock<std::mutex> lk(m_logMutex);
      String message = Utils::format("New stage created: %s", fileName.c_str());
      std::cout << message << std::endl;
      Logger::instance().logDebug(message, LOG_CHANNEL::E::eSYSTEM);
    }

    // Always a good idea to declare your up-ness
    UsdGeomSetStageUpAxis(m_stage, UsdGeomTokens->y);
    return fileName;
  }

  bool
  Omniverse::deleteUSD(const String& fileName) {
    
    // Delete the old version of this file on Omniverse and wait for the operation to complete
    {
      UniqueLock<Mutex> lk(m_logMutex);
      String message = Utils::format("Waiting for %s to delete...", fileName.c_str());
      std::cout << message << std::endl;
      Logger::instance().logDebug(message, LOG_CHANNEL::E::eSYSTEM);
    }
    omniClientWait(omniClientDelete(fileName.c_str(), nullptr, nullptr));
    {
      std::unique_lock<std::mutex> lk(m_logMutex);
      String message = Utils::format("Finished deleting %s", fileName.c_str());
      std::cout << message << std::endl;
      Logger::instance().logDebug(message, LOG_CHANNEL::E::eSYSTEM);
      return true;
    }
  }

  void
  Omniverse::closeUSD() {
    UpdateUSD();
    m_stage.Reset();
  }

  void
  Omniverse::saveUSD() {
    m_stage->Save();

    if (isDefaultLiveSyncEnabled()) {
      omniUsdLiveProcess();
    }
  }

  void
  Omniverse::saveToUSD(const String& fileName) {

    saveUSD();
  }

  void
  Omniverse::UpdateUSD() {
  
  }

  void
  Omniverse::checkpointFile(const String& url, const String& comment) {
    if (omniUsdLiveGetDefaultEnabled()) {
      return;
    }

    bool bCheckpointsSupported = false;
    omniClientWait(omniClientGetServerInfo(url.c_str(), &bCheckpointsSupported,
      [](void* UserData, OmniClientResult Result, OmniClientServerInfo const* Info) noexcept
      {
        if (Result == eOmniClientResult_Ok && Info && UserData)
        {
          bool* bCheckpointsSupported = static_cast<bool*>(UserData);
          *bCheckpointsSupported = Info->checkpointsEnabled;
        }
      }));

    if (bCheckpointsSupported)
    {
      const bool bForceCheckpoint = true;
      omniClientWait(
        omniClientCreateCheckpoint(url.c_str(), 
                                   comment.c_str(), 
                                   bForceCheckpoint, 
                                   nullptr,
                                   [](void* userData, 
                                      OmniClientResult result, 
                                      char const* checkpointQuery) noexcept
        {}));

      UniqueLock<Mutex> lk(m_logMutex);
      String message = Utils::format("Adding checkpoint comment <%s> to stage <%s>", comment, url);
      std::cout << message << std::endl;
      Logger::instance().logDebug(message, LOG_CHANNEL::E::eSYSTEM);

    }
  }

  String
  Omniverse::getActiveUsername(const String& stageUrl) {
    String userName("_none_");
    omniClientWait(omniClientGetServerInfo(stageUrl.c_str(), &userName, [](void* userData, OmniClientResult result, struct OmniClientServerInfo const* info) noexcept
      {
        std::string* userName = static_cast<std::string*>(userData);
        if (userData && userName && info && info->username)
        {
          userName->assign(info->username);
        }
      }));
    {
      std::unique_lock<std::mutex> lk(m_logMutex);
      String message = Utils::format("Connected username: %s", userName.c_str());
      std::cout << message << std::endl;
      Logger::instance().logDebug(message, LOG_CHANNEL::E::eSYSTEM);
    }
    return userName;
  }

  bool
  Omniverse::isValidOmniURL(const String& filePath) {
    bool isValidURL = false;
    OmniClientUrl* url = omniClientBreakUrl(filePath.c_str());
    if (url->host && url->path && 
        (String(url->scheme) == String("omniverse") || String(url->scheme) == String("omni")))
    {
      isValidURL = true;
    }
    omniClientFreeUrl(url);
    return isValidURL;
  }

  File
  Omniverse::downloadFile(const String& filePath) {
    return File();
  }

  bool
  Omniverse::uploadFile(const String& filePath, const String& destiny) {
    omniClientWait(omniClientCopy(filePath.c_str(), destiny.c_str(), copyCallback, nullptr));
    {
      UniqueLock<Mutex> lk(m_logMutex);
      std::cout << "finished" << std::endl;
      return true;
    }
  }

  File
  Omniverse::downloadMaterial(const String& materialPath) {
    return File();
  }

  bool
  Omniverse::uploadMaterial(const String& materialPath, const String& destiny) {

    return true;
  }

  bool
  Omniverse::isDefaultLiveSyncEnabled() {
    return omniUsdLiveGetDefaultEnabled();
  }

  const OMNILIVEMODE::E&
  Omniverse::isStageLiveSyncEnabled(const String& url) {
    return OMNILIVEMODE::E::_from_integral(omniUsdLiveGetModeForUrl(url.c_str()));
  }

  bool
  Omniverse::isOVLogEnabled() {
    return m_omniverseLogEnabled;
  }

  void
  Omniverse::setDefaultLiveSync(bool newStatus) {
    omniUsdLiveSetDefaultEnabled(newStatus);
  }

  void
  Omniverse::setStageLiveSync(const String& url, const OMNILIVEMODE::E& newStatus) {
    omniUsdLiveSetModeForUrl(url.c_str(), static_cast<OmniUsdLiveMode>(newStatus._to_integral()));
  }

  void
  Omniverse::setOVLog(const OMNILOGLEVEL::E& logLevel) {
    omniClientSetLogLevel(static_cast<OmniClientLogLevel>(logLevel._to_integral()));
  }

  void
  Omniverse::waitForUpdates() {
    omniUsdLiveWaitForPendingUpdates();
  }

  String
  Omniverse::getVersion() {
    return omniClientGetVersionString();
  }

  const String&
  Omniverse::getLog() {
    return m_message;
  }
}

