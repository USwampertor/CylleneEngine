#pragma once
#include "cyCorePrerequisites.h"
#include <cstddef>
;

namespace CYLLENE_SDK {

  // Values are defined as #include so its not a pain in the ass
  namespace DEFAULTICONS {

  static const uint8 cameraIcon[] = 
  {
#include "assets/cyCameraIcon.h";
  };

  static const uint8 lightIcon[] = {
#include "assets/cyLightIcon.h";
  };

  static const uint8 missingTex[] = { 
#include "assets/cyMissingTexture.h" 
  };

  static const uint8 atlasIcon[]  = { 
#include "assets/cyAtlasIcon.h";
  };

  static const uint8 jsonIcon[]   = { 
#include "assets/cyJSONIcon.h";
  };

  }

  namespace DEFAULTMISC {
  static const uint8 kimchi[] = {
#include "assets/cyKimchi.h";
  };

  static const uint8 checkers[] = {
#include "assets/cyChecker.h";
  };
    
  static const uint8 moon[] = {
#include "assets/cyMoon.h";
  };
  
  }

  namespace DEFAULTSHADERS {
  static const String defaultShader = {
#include "assets/cyDefaultShader.h"
  };

  }

  namespace DEFAULTMATERIALS {
    static const String defaultMaterial = {
#include "assets/cyDefaultMaterial.h" 
    };
  }

}