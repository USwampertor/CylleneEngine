/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyLiveSessionInfo.h
 * @author Marco "Swampy" Millan
 * @date 20/09/2022
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once
#include "cyOmniversePrerequisites.h"

namespace CYLLENE_SDK
{
  /*
 * Live Session Info class
 *
 * This class attempts to collect all of the logic around the live session file paths and URLs.
 * It should be first instantiated with the stage URL (omniverse://server/folder/stage.usd), then
 * GetSessionFolderPathForStage() can be used to list the available sessions.
 *
 * Once a session is selected, SetSessionName() will finish the initialization of all of the paths
 * and the other methods can be used.
 *
 * In the folder that contains the USD to be live-edited, there exists this folder structure:
 * <.live> / < my_usd_file.live> / <session_name.live> / root.live

 * GetSessionFolderPathForStage:   <stage_folder> / <.live> / <my_usd_file.live>
 * GetLiveSessionFolderPath:       <stage_folder> / <.live> / <my_usd_file.live> / <session_name.live>
 * GetLiveSessionUrl:              <stage_folder> / <.live> / <my_usd_file.live> / <session_name.live> / root.live
 * GetLiveSessionTomlUrl:          <stage_folder> / <.live> / <my_usd_file.live> / <session_name.live> / __session__.toml
 * GetMessageChannelUrl:           <stage_folder> / <.live> / <my_usd_file.live> / <session_name.live> / __session__.channel
*/

  class LiveSessionInfo
  {
  public:
    const char* OMNIVERSE_CHANNEL_FILE_NAME = "__session__.channel";
    const char* LIVE_SUBFOLDER = "/.live";
    const char* LIVE_SUBFOLDER_SUFFIX = ".live";
    const char* DEFAULT_LIVE_FILE_NAME = "root.live";
    const char* SESSION_TOML_FILE_NAME = "__session__.toml";

    LiveSessionInfo() {}
    LiveSessionInfo(const char* stageUrl);
    virtual ~LiveSessionInfo() {}

    // Initialize the live session info pahts
    void 
    Initialize(const char* stageUrl);
    
    String 
    BuildUrlString(const char* path);
    
    String 
    GetSessionFolderPathForStage();

    // Much of the class methods don't function correctly until this method is called
    // Session names must start with an alphabetical character, but may contain alphanumeric,
    //  hyphen, or underscore characters.
    // 
    // Returns true if the name is valid, false if it contains invalid characters
    bool 
    SetSessionName(const char* session_name);

    // Get the live session name
    String 
    GetSessionName() { return mSessionName; }

    // This checks for the existance of the session config (TOML) file
    bool 
    DoesSessionExist();

    // This returns the absolute path of the live session folder where live session files live
    // * root.live           - the live layer with the live session USD contents
    // * __session__.toml    - the session config file
    // * __session__.channel - the omni channel file
    String 
    GetLiveSessionFolderPath();

    // Returns a list of existing sessions for the stage
    Vector<String> 
    GetLiveSessionList();

    // Returns the root stage URL for the live session
    String 
    GetStageUrl() { return mStageUrlStr; }

    // Get the URL for the folder in which the stage resides
    String 
    GetStageFolderUrl();

    // Get the filename of the root stage
    String 
    GetStageFileName();

    // Get the URL for the .live layer file
    String 
    GetLiveSessionUrl();

    // Get the URL for the TOML configuration file
    String 
    GetLiveSessionTomlUrl();

    // Get the URL for the Omniverse Channel file
    String 
    GetMessageChannelUrl();

    // A utility method for the class or any application - will report if a file exists
    // on local disk or on a Nucleus server
    bool 
    fileExists(const char* fileUrl);

  private:
    struct OmniClientUrl* mOmniUrl = nullptr;
    String mStageUrlStr;
    String mStageFolderUrlStr;
    String mLiveFileUrlStr;
    String mSessionName;
    String mOmniSessionFolderPath;
    String mSessionFolderString;
  };

}
