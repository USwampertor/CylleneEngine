#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyUtilities.h"

namespace CYLLENE_SDK {
class CY_UTILITY_EXPORT ArgumentParser
{
public:
  ArgumentParser() = default;

  ~ArgumentParser() = default;
  
  void
  parse(const String& parameters);

  bool
  addArgument(const String& newArgument);

  String
  getParameter(const String& parameter);

  Map<String, String> m_argumentMap;

};
}
