#pragma once

#include "cyCorePrerequisites.h"

#include "cyPlugin.h"

#include <cyModule.h>

namespace CYLLENE_SDK {

class PluginManager : public Module<PluginManager>
{

  void
  loadPlugin(const String& pluginName);

  Vector<SharedPointer<Plugin>> m_plugins;

  Map<String, SharedPointer<Plugin>> m_essentialPlugins;

};


// Plugins should be initialized by looking a function called registerplugin


}

//  K5J8C-VI8DY-XP7HT