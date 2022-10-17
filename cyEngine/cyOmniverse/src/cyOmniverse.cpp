#include "cyOmniverse.h"

#include <cppUtils/primUtils.h>
#include <cppUtils/xformUtils.h>

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
      omniClientLiveProcess();
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
  Omniverse::checkpointFile(const String& url, const String& comment, bool forceCheckpoint) {
    // if (omniUsdLiveGetDefaultEnabled()) {
    //   return;
    // }

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
      omniClientWait(
        omniClientCreateCheckpoint(url.c_str(), 
                                   comment.c_str(), 
                                   forceCheckpoint, 
                                   nullptr,
                                   [](void* userData, 
                                      OmniClientResult result, 
                                      char const* checkpointQuery) noexcept
        {}));

      UniqueLock<Mutex> lk(m_logMutex);
      String message = Utils::format("Adding checkpoint comment <%s> to stage <%s>", comment, url);
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
    omniClientWait(omniClientCopy(filePath.c_str(), destiny.c_str(), omniCopyCallback, nullptr));
    {
      UniqueLock<Mutex> lk(m_logMutex);
      Logger::instance().logDebug(Utils::format("Finished copying file %s", filePath));
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

  MDLData
  Omniverse::exportMDL(MDLInfo info) {
    MDLData data;


    return data;
  }

  MDLInfo
  Omniverse::importMDL(MDLData data) {
    MDLInfo info;

    return info;
  }

  bool
  Omniverse::isDefaultLiveSyncEnabled() {
    // return omniUsdLiveGetDefaultEnabled();
    return m_liveEnabled;
  }

  const OMNILIVEMODE::E
  Omniverse::isStageLiveSyncEnabled(const String& url) {
    // return OMNILIVEMODE::E::_from_integral(omniUsdLiveGetModeForUrl(url.c_str()));
    return url.find_last_of(".live") != String::npos ? 
      OMNILIVEMODE::E::eENABLED : 
      OMNILIVEMODE::E::eDISABLED;
  }

  bool
  Omniverse::isOVLogEnabled() {
    return m_omniverseLogEnabled;
  }

  void
  Omniverse::setDefaultLiveSync(bool newStatus) {
    // omniUsdLiveSetDefaultEnabled(newStatus);
    m_liveEnabled = newStatus;
  }

  void
  Omniverse::setStageLiveSync(const String& url, const OMNILIVEMODE::E& newStatus) {
    // omniUsdLiveSetModeForUrl(url.c_str(), static_cast<OmniUsdLiveMode>(newStatus._to_integral()));
  }

  void
  Omniverse::setOVLog(const OMNILOGLEVEL::E& logLevel) {
    omniClientSetLogLevel(static_cast<OmniClientLogLevel>(logLevel._to_integral()));
  }

  void
  Omniverse::waitForUpdates() {
    omniClientLiveWaitForPendingUpdates();
  }

  bool
  Omniverse::isSessionNameValid(const String& newSessionName) {
    return m_liveSessionInfo.SetSessionName(newSessionName.c_str());
  }

  Vector<String>
  Omniverse::getSessions() {
    String sessionFolderForStage = m_liveSessionInfo.GetSessionFolderPathForStage();
    Vector<String> sessions = m_liveSessionInfo.GetLiveSessionList();

    // Get the live layer from the live stage
    SdfLayerHandle liveLayer = m_stage->GetRootLayer();

    // Construct the layers so that we can join the session
    m_stage->GetSessionLayer()->InsertSubLayerPath(liveLayer->GetIdentifier());
    m_stage->SetEditTarget(UsdEditTarget(liveLayer));
    return sessions;
  }

  bool
  Omniverse::joinSession(const String& sessionName) {
    m_liveSessionInfo.SetSessionName(sessionName.c_str());


    LiveSessionConfigFile sessionConfig;
    std::string tomlUrl = m_liveSessionInfo.GetLiveSessionTomlUrl();
    if (!sessionConfig.IsVersionCompatible(tomlUrl.c_str())) {
      String actualVersion = sessionConfig.GetSessionConfigValue(tomlUrl.c_str(), OMNIKEY::E::eVERSION);
      String message = 
        Utils::format("The session config TOML file version is not compatible. \nExpected: %s Actual: %s",
                      LiveSessionConfigFile::kCurrentVersion, actualVersion);
      Logger::instance().log(message);
      return false;
    }

    String liveSessionUrl = m_liveSessionInfo.GetLiveSessionUrl();
    m_stage = UsdStage::Open(liveSessionUrl);

    // Get the live layer from the live stage
    SdfLayerHandle liveLayer = m_stage->GetRootLayer();

    // Construct the layers so that we can join the session
    m_stage->GetSessionLayer()->InsertSubLayerPath(liveLayer->GetIdentifier());
    m_stage->SetEditTarget(UsdEditTarget(liveLayer));
    return true;
  }

  bool
  Omniverse::createSession(const String& newSessionName) {
    
    if (!isSessionNameValid(newSessionName)) {

      String message =
        Utils::format(
"Session name %s \n\
Session names must start with an alphabetical character \
but may contain alphanumeric, hyphen, or underscore characters.", newSessionName);
      Logger::instance().logError(message);
      return false;
    }

    // Make sure that this session doesn't already exist (don't overwrite/stomp it)
    if (m_liveSessionInfo.DoesSessionExist()) {
      Logger::instance().logError(Utils::format("Session config file already exists at %s", 
                                  m_liveSessionInfo.GetLiveSessionTomlUrl()));
      return false;
    }

    // Create the session config file 
    Map<OMNIKEY::E, const char*> keyMap;
    std::string stageUrl = m_liveSessionInfo.GetStageUrl();
    std::string connectedUserName = getActiveUsername(stageUrl.c_str());
    keyMap[OMNIKEY::E::eADMIN] = connectedUserName.c_str();
    keyMap[OMNIKEY::E::eSTAGEURL] = stageUrl.c_str();
    keyMap[OMNIKEY::E::eMODE] = "default";
    LiveSessionConfigFile sessionConfig;

    if (!sessionConfig.CreateSessionConfigFile(m_liveSessionInfo.GetLiveSessionTomlUrl().c_str(), keyMap))
    {
      Utils::format("Unable to create session config file: %s", 
                    m_liveSessionInfo.GetLiveSessionTomlUrl());
      return false;
    }

    // Create the new root.live file to be the stage's edit target
    String liveSessionUrl = m_liveSessionInfo.GetLiveSessionUrl();
    m_stage = UsdStage::CreateNew(liveSessionUrl);

    // Get the live layer from the live stage
    SdfLayerHandle liveLayer = m_stage->GetRootLayer();

    // Construct the layers so that we can join the session
    m_stage->GetSessionLayer()->InsertSubLayerPath(liveLayer->GetIdentifier());
    m_stage->SetEditTarget(UsdEditTarget(liveLayer));
    return true;
  } 
  
  bool
  Omniverse::endSession(const OMNIMERGEOPTIONS::E& mergeOption) {
    
    LiveSessionConfigFile sessionConfig;
    String sessionAdmin = sessionConfig.GetSessionAdmin(m_liveSessionInfo.GetLiveSessionTomlUrl().c_str());
    String currentUser = getActiveUsername(m_liveSessionInfo.GetStageUrl().c_str());
    if (sessionAdmin != currentUser)
    {
      Logger::instance().logError(Utils::format("You [%s] are not the session admin [%s]. Stopping merge", 
                                                currentUser, 
                                                sessionAdmin));
      return false;
    }

    // Gather the latest changes from the live stage
    omniClientLiveProcess();

    // Send a MERGE_STARTED channel message
    m_omniChannel.SendChannelMessage(OMNIMESSAGETYPE::E::eMERGESTARTED);

    // Create a checkpoint on the live layer (don't force if no changes)
    // Create a checkpoint on the root layer (don't force if no changes)
    std::string comment("Pre-merge for " + m_liveSessionInfo.GetSessionName() + " session");
    checkpointFile(m_liveSessionInfo.GetLiveSessionUrl(), comment.c_str(), false);
    checkpointFile(m_liveSessionInfo.GetStageUrl(), comment.c_str(), false);

    return OMNIMERGEOPTIONS::E::eNOMERGE ? true : mergeSession(mergeOption);
  }

  bool
  Omniverse::mergeSession(const OMNIMERGEOPTIONS::E& mergeOption) {
    if (OMNIMERGEOPTIONS::E::eNEWLAYER ==  mergeOption)
    {
      // Inject a new layer in the same folder as the root with the session name into the root stage (rootStageName_sessionName_edits.usd)
      String stageName = m_liveSessionInfo.GetStageFileName();
      String stageFolder = m_liveSessionInfo.GetStageFolderUrl();
      String sessionName = m_liveSessionInfo.GetSessionName();
      String newLayerUrl = stageFolder + "/" + stageName + "_" + sessionName + ".usd";
      
      Logger::instance().logDebug(
        Utils::format("Merging session changes to %s and inserting as a sublayer in the root layer.", 
                      newLayerUrl));
      
      // TODO: SOLVE THIS
      // primUtils::MergeLiveLayerToNewLayer(m_stage->GetEditTarget().GetLayer(),
      //                                     m_stage->GetRootLayer(), 
      //                                     newLayerUrl.c_str());
    }
    else if (OMNIMERGEOPTIONS::E::eROOTLAYER == mergeOption)
    {
      // Merge the live deltas to the root layer
      // This does not clear the source layer --- we'll do that after checkpointing it
      
      // TODO: SOLVE THIS
      // primUtils::MergeLiveLayerToRoot(m_stage->GetEditTarget().GetLayer(), 
      //                                 m_stage->GetRootLayer());
    }

    // Create a checkpoint on the root layer while saving it
    String postComment("Post-merge for " + m_liveSessionInfo.GetSessionName() + " session");
    Logger::instance().logDebug(postComment);
    omniUsdResolverSetCheckpointMessage(postComment.c_str());
    m_stage->GetRootLayer()->Save();
    omniUsdResolverSetCheckpointMessage("");

    // Clear and save the live layer
    // We shouldn't need to save this live layer, there's a bug somewhere between the 
    // USD resolver, client library, or Nucleus server.  If we don't the contents won't
    // be cleared.
    m_stage->GetEditTarget().GetLayer()->Clear();
    m_stage->GetEditTarget().GetLayer()->Save();

    // Remove the .live layer from the session layer
    m_stage->GetSessionLayer()->GetSubLayerPaths().clear();

    // Send a MERGE_FINISHED channel message
    m_omniChannel.SendChannelMessage(OMNIMESSAGETYPE::E::eMERGEFINISHED);

    return true;
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

