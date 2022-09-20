#include "cyLiveSessionInfo.h"
#include <cyFileSystem.h>

namespace CYLLENE_SDK
{

LiveSessionInfo::LiveSessionInfo(const char* stage_url) {
  Initialize(stage_url);
}

void 
LiveSessionInfo::Initialize(const char* stageUrl) {
  mStageUrlStr = stageUrl;
  mOmniUrl = omniClientBreakUrl(stageUrl);
  // construct the folder that would contain sessions - <.live> / < my_usd_file.live> / <session_name> / root.live
  // self.omni_session_folder_path = os.path.dirname(self.omni_url.path) + self.LIVE_SUBFOLDER + "/" + self.get_stage_file_name() + self.LIVE_SUBFOLDER_SUFFIX
  // self.session_folder_string = omni.client.make_url(self.omni_url.scheme, self.omni_url.user, self.omni_url.host, self.omni_url.port, self.omni_session_folder_path
  Path p(mOmniUrl->path);

  // Because std::experimental::filesystem normalizes the path (on Windows) and we need forward slashes
  // String parent_path_str = p.parent_path().string();
  // std::replace(parent_path_str.begin(), parent_path_str.end(), '\\', '/');
  mStageFolderUrlStr = BuildUrlString(p.directoryPath().c_str());
  
  mOmniSessionFolderPath = p.fullPath() + LIVE_SUBFOLDER + "/" + GetStageFileName() + LIVE_SUBFOLDER_SUFFIX;
  mSessionFolderString = BuildUrlString(mOmniSessionFolderPath.c_str());
}

String 
LiveSessionInfo::BuildUrlString(const char* path) {
  String urlString;
  OmniClientUrl omniUrl = *mOmniUrl;
  omniUrl.path = path;

  size_t expectedSize = 0;
  omniClientMakeUrl(&omniUrl, nullptr, &expectedSize);
  char* buf = new char[expectedSize];
  omniClientMakeUrl(&omniUrl, buf, &expectedSize);
  urlString.assign(buf);
  delete[] buf;
  return urlString;
}

String LiveSessionInfo::GetSessionFolderPathForStage() {
  return mSessionFolderString;
}

bool LiveSessionInfo::SetSessionName(const char* sessionName)
{
  // Session names must start with an alphabetical character, but may contain alphanumeric, hyphen, or underscore characters.
  Regex re("^[a-zA-Z][a-zA-Z0-9_-]+");
  CMatch match;
  if (std::regex_match(sessionName, match, re))
  {
    mSessionName.assign(sessionName);
    return true;
  }
  else
  {
    return false;
  }
}

bool LiveSessionInfo::DoesSessionExist() {
  return fileExists(GetLiveSessionTomlUrl().c_str());
}

String LiveSessionInfo::GetLiveSessionFolderPath() {
  return mOmniSessionFolderPath + "/" + mSessionName + LIVE_SUBFOLDER_SUFFIX;
}

Vector<String> LiveSessionInfo::GetLiveSessionList() {
  String sessionFolderForStage = GetSessionFolderPathForStage();
  Vector<String> folders;
  omniClientWait(omniClientList(sessionFolderForStage.c_str(), &folders,
                 [](void* userData, 
                   OmniClientResult result, 
                   uint32_t numEntries, 
                   struct OmniClientListEntry 
                   const* entries) noexcept {
    Vector<String>* foldersPtr = static_cast<Vector<String>*>(userData);
    for (uint32_t i = 0; i < numEntries; i++) {
      foldersPtr->push_back(entries[i].relativePath);
    }
    }));

  Vector<String> sessionNames;
  for (int i = 0; i < folders.size(); i++) {
    Path p(folders[i]);
    sessionNames.push_back(p.baseName());
  }
  return sessionNames;
}

String LiveSessionInfo::GetStageFolderUrl() {
  return mStageFolderUrlStr;
}

String LiveSessionInfo::GetStageFileName()
{
  // find the stage file's root name
  Path p(mOmniUrl->path);
  return p.baseName();
}

String LiveSessionInfo::GetLiveSessionUrl()
{
  String live_session_path = GetLiveSessionFolderPath() + "/" + DEFAULT_LIVE_FILE_NAME;
  return BuildUrlString(live_session_path.c_str());
}

String LiveSessionInfo::GetLiveSessionTomlUrl()
{
  String live_session_toml_path = GetLiveSessionFolderPath() + "/" + SESSION_TOML_FILE_NAME;
  return BuildUrlString(live_session_toml_path.c_str());
}

String LiveSessionInfo::GetMessageChannelUrl()
{
  String live_session_channel_path = GetLiveSessionFolderPath() + "/" + OMNIVERSE_CHANNEL_FILE_NAME;
  return BuildUrlString(live_session_channel_path.c_str());
}

bool LiveSessionInfo::fileExists(const char* fileUrl)
{
  bool bFileExists = false;
  omniClientWait(omniClientStat(
    fileUrl,
    &bFileExists,
    [](void* userData, 
       OmniClientResult result, 
       struct OmniClientListEntry 
       const* entry) noexcept {
      bool* fileExists = static_cast<bool*>(userData);
      *fileExists = (result == eOmniClientResult_Ok);
    }
  ));
  return bFileExists;
}
}