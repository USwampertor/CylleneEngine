#pragma once

#include "cyCorePrerequisites.h"

#include <cyArgumentParser.h>
#include <cyModule.h>

namespace CYLLENE_SDK {

class CY_CORE_EXPORT App : public Module<App>
{
public:


  App() = default;

  App(const String& params) {
    m_parser.parse(params);
  }

  App(int argc, const char* argv[]) {
    m_parser.parse(argc, argv);
  }

  virtual bool
  startSystems() = 0;

  virtual bool
  loadResources() = 0;

  virtual void
  run() = 0;

  virtual void
  update() = 0;

  virtual void
  exit() = 0;

  ArgumentParser m_parser;

};

}

