#pragma once

#include "cyScriptingPrerequisites.h"
#include <cyModule.h>


namespace CYLLENE_SDK {

class ScriptCompiler : public Module<ScriptCompiler>
{
  bool
  diff();

  List<String>
  fetch();

  bool
  write();

  bool
  compile();
  

};

}

