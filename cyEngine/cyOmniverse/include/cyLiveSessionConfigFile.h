#pragma once

#include "cyOmniversePrerequisites.h"

namespace CYLLENE_SDK {
  /*
 * Live Session Config File class
 *
 * This class is intended to assist in reading and writing the live session configuration file.
 *
 * The config file is TOML-formatted and is primarily used to denote the owner, or admin of the
 * live session.
 *
 * possible modes - "default" = "root_authoring", "auto_authoring", "project_authoring"
*/
  namespace OMNIKEY
  {
    BETTER_ENUM(E, uint32, 
                eVERSION, 
                eADMIN, 
                eSTAGEURL, 
                eDESCRIPTION, 
                eMODE, eNAME, 
                eINVALID);

  }

class LiveSessionConfigFile
{
public:

  // using KeyMap = Map<OMNIKEY::E, const char*>;
  static constexpr const char* kCurrentVersion = "1.0";

  LiveSessionConfigFile() {};
  ~LiveSessionConfigFile() {};

  String 
  GetSessionConfigValue(const String& sessionConfigFileUrl, OMNIKEY::E key);
  
  bool 
  CreateSessionConfigFile(const String& sessionConfigFileUrl, 
                          const Map<OMNIKEY::E, const char*>& keyMap);
  
  String 
  GetSessionAdmin(const char* sessionConfigFileUrl);

  /* Check that a version is compatible [major.minor]
    * If major is the same, return true, else return false
    * This works under the assumption that future minor versions will still work
    */
  bool 
  IsVersionCompatible(const char* sessionConfigFileUrl);

  String 
  KeyToString(OMNIKEY::E key);
  
  OMNIKEY::E 
  StringToKey(const String& keyString);
};


}
