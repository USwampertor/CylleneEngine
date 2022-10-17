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


// #include "cyPrimUtils.h"
// #include "cyXformUtils.h"
// 

namespace CYLLENE_SDK
{

class Material;

//TODO: Add more flexibility to this so Materials are easier to Use
struct MDLTextureInfo
{


};

struct CY_OMNIVERSE_EXPORT MDLInfo
{
  String id;
  String name;
  String description;
  String version;
  String author;
  
  Vector<String> keyWords;
  String diffuse;
  String normal;
  String albedo;
  String metal;
  String orm;
  String roughness;
  String emissive;
  String ao;
};

struct CY_OMNIVERSE_EXPORT MDLData
{
  String id;
  String name;
  String path;
  void* data;
};


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

namespace OMNIMERGEOPTIONS
{
  BETTER_ENUM(E, uint32, 
    eNEWLAYER,
    eROOTLAYER,
    eNOMERGE)
}

namespace OMNICONNECTIONSTATUS
{
  BETTER_ENUM(E, uint32,
    eCONNECTIONSTATUSCONNECTING,
    eCONNECTIONSTATUSCONNECTED,
    eCONNECTIONSTATUSCONNECTERROR,
    eCONNECTIONSTATUSDISCONNECT);
}

class CY_OMNIVERSE_EXPORT ChannelUpdate
{
  public:
    ChannelUpdate() = default;
    ~ChannelUpdate() = default;

    ChannelUpdate(int updatePeriodMs)
      : mUpdatePeriodMs(updatePeriodMs),
        mStopped(false) {}

    void run() {
      while (!mStopped) {
        std::this_thread::sleep_for(std::chrono::milliseconds(mUpdatePeriodMs));
        for (OmniChannel* channel : mChannels) {
          channel->Update();
        }
      }
    }

    int mUpdatePeriodMs;
    bool mStopped;
    Vector<OmniChannel*> mChannels;
};


class CY_OMNIVERSE_EXPORT Omniverse : public Module<Omniverse> {
public:
  
  Omniverse() = default;
  ~Omniverse() = default;

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
  omniCopyCallback() {

  }

  static void
  omniMergeCallback(OMNIMESSAGETYPE::E messageType, 
                    void* userData, 
                    const char* userName, 
                    const char* appName) {
    // Just a note that userData is available for context
    String* existingStage = static_cast<String*>(userData);
    String message = Utils::format("Channel Callback: %s %s - %s", 
                                   OmniChannelMessage::MessageTypeToStringType(messageType), 
                                   userName, 
                                   appName);
    std::cout << message;
    Logger::instance().logDebug(message);
    if (messageType == OMNIMESSAGETYPE::E::eMERGESTARTED||
        messageType == OMNIMESSAGETYPE::E::eMERGEFINISHED)
    {
      std::cout << "Exiting since a merge is happening in another client" << std::endl;
      Omniverse::instance().m_stageMerged = true;
    }
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
  checkpointFile(const String& url, const String& comment, bool forceCheckpoint = true);

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

  MDLData
  exportMDL(MDLInfo info);

  MDLInfo
  importMDL(MDLData data);

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

  bool
  isSessionNameValid(const String& newSessionName);
  
  Vector<String>
  getSessions();

  bool
  joinSession(const String& sessionName);

  bool
  createSession(const String& newSessionName);
  
  bool
  endSession(const OMNIMERGEOPTIONS::E& mergeOption);

  bool
  mergeSession(const OMNIMERGEOPTIONS::E& mergeOption);

  String
  getVersion();

  const String&
  getLog();

private:
  // bool            m_doLiveEdit = true;
  bool            m_omniverseLogEnabled = false;
  bool            m_liveEnabled;
  bool            m_stageMerged;

  String          m_message;
  std::mutex      m_logMutex;

  UsdStageRefPtr  m_stage;
  OmniChannel     m_omniChannel;
  LiveSessionInfo m_liveSessionInfo;

  std::thread m_channelUpdateThread;
  ChannelUpdate m_channelUpdater;
};
}
