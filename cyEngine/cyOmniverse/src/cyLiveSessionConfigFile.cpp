#include "cyLiveSessionConfigFile.h"
#include <toml/toml.h>

namespace CYLLENE_SDK {
  String LiveSessionConfigFile::GetSessionConfigValue(const char* sessionConfigFileUrl, 
                                                      OMNIKEY::E key) {
    String retStr("");
    toml::Value tomlValue;

    // This is a callback function for when the file is read from Nucleus
    // it will parse the contents of the toml file and 
    auto readFileCallback = [](void* userData, 
                               OmniClientResult result, 
                               char const* version, 
                               struct OmniClientContent* content) noexcept {
      if (result == OmniClientResult::eOmniClientResult_Ok && 
          content && 
          userData) {
        toml::Value* tomlValuePtr = static_cast<toml::Value*>(userData);
        IStringStream ss(String((char*)content->buffer, content->size));

        toml::ParseResult pr = toml::parse(ss);
        if (!pr.valid()) {
          std::cout << "Reading the Session Config file failed (toml parse): " << 
                       pr.errorReason << 
                       std::endl;
          return;
        }
        *tomlValuePtr = pr.value;
      }
    };

    // Read the file from Nucleus
    omniClientWait(omniClientReadFile(sessionConfigFileUrl, &tomlValue, readFileCallback));

    // We should verify that the version is compatible here...

    const toml::Value* x = tomlValue.find(KeyToString(key));
    if (x && x->is<String>()) {
      retStr = x->as<String>();
    }
    return retStr;
  }

  String 
  LiveSessionConfigFile::GetSessionAdmin(const char* sessionConfigFileUrl) {
    return GetSessionConfigValue(sessionConfigFileUrl, OMNIKEY::E::eADMIN);
  }

  bool 
  LiveSessionConfigFile::CreateSessionConfigFile(const char* sessionConfigFileUrl, 
                                                 const Map<OMNIKEY::E, const char*>& keyMap) {
    bool bFileWriteSuccess = false;
    toml::Value root((toml::Table()));
    for (const auto& key : keyMap) {
      root.setChild(KeyToString(key.first), key.second);
    }

    // Add a version
    root.setChild(KeyToString(OMNIKEY::E::eVERSION), kCurrentVersion);

    OStringStream oss;
    root.write(&oss);
    OmniClientContent content = omniClientAllocContent(oss.str().length());
    std::memcpy(content.buffer, oss.str().c_str(), content.size);
    omniClientWait(omniClientWriteFile(
      sessionConfigFileUrl,
      &content,
      &bFileWriteSuccess,
      [](void* userData, OmniClientResult result) noexcept {
        bool* success = static_cast<bool*>(userData);
        *success = (result == OmniClientResult::eOmniClientResult_Ok);
      }));

    return bFileWriteSuccess;
  }

  bool 
  LiveSessionConfigFile::IsVersionCompatible(const char* sessionConfigFileUrl) {
    String configVersion = GetSessionConfigValue(sessionConfigFileUrl, OMNIKEY::E::eVERSION);
    if (configVersion.empty()) {
      return false;
    }
    else {
      // Get the TOML file major version
      std::istringstream tokenStream(configVersion);
      String configMajorVersion;
      std::getline(tokenStream, configMajorVersion, '.');

      // Get the _current_ major version
      IStringStream tokenStream2(kCurrentVersion);
      String currentMajorVersion;
      std::getline(tokenStream2, currentMajorVersion, '.');

      return (configMajorVersion == currentMajorVersion);
    }
  }

  String 
  LiveSessionConfigFile::KeyToString(OMNIKEY::E key) {
    return key._to_string();
  }

  OMNIKEY::E 
  LiveSessionConfigFile::StringToKey(const String& keyString) {
    return OMNIKEY::E::_from_string_nothrow(keyString.c_str()) ?
            OMNIKEY::E::_from_string(keyString.c_str()) : 
            OMNIKEY::E::eINVALID;
  }

}