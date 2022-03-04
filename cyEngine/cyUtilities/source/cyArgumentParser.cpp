
#include "cyArgumentParser.h"

namespace CYLLENE_SDK {

void
ArgumentParser::parse(const String& parameters) {
  for (auto argument : m_argumentMap) {
    if (parameters.find(argument.first) != String::npos) {
      String tmpString = Utils::BLANK;
      for (int32 i = static_cast<int32>(parameters.find(argument.first)) + argument.first.length() + 1; i < parameters.length(); ++i) {
        if (parameters[i] == '-') { break; }
        tmpString += parameters[i];
      }
      m_argumentMap[argument.first] = tmpString;
    }
  }
}

bool
ArgumentParser::addArgument(const String& newArgument) {
  if (m_argumentMap.find(newArgument) == m_argumentMap.end()) {
    m_argumentMap.insert(std::make_pair(newArgument, Utils::BLANK));
    return true;
  }
  return false;
}

String
ArgumentParser::getParameter(const String& parameter) {
  if (m_argumentMap.find(parameter) != m_argumentMap.end()) {
    return m_argumentMap[parameter];
  }
  return Utils::BLANK;
}

}
