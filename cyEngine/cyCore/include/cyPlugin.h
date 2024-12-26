#pragma once
#include "cyCorePrerequisites.h"

namespace CYLLENE_SDK {

class Plugin
{
public:

  Plugin() = default;

  virtual ~Plugin() {}

  uint64 m_id;
  String m_name;
  String m_company;
  String m_description; 

  virtual void*
  initializePlugin() = 0;

};

}

