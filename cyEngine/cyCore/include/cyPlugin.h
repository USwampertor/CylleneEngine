#pragma once
#include "cyCorePrerequisites.h"

namespace CYLLENE_SDK {

class Plugin
{
public:

  Plugin() = default;

  virtual ~Plugin() {}

  static uint64 m_id;
  static String m_name;
  static String m_company;
  static String m_description; 

  virtual bool
  initialize() = 0;

  virtual bool
  uninitialize() = 0;

};

}

