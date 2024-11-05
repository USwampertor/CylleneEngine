
#include "cyArgumentParser.h"

namespace CYLLENE_SDK {


void
ArgumentParser::parse(const int& argc, char* argv[])
{

}

void
ArgumentParser::parse(const String& parameters) {
  for (auto argument : m_flagMap) {
    if (parameters.find(argument.first) != String::npos) {
      String tmpString = Utils::BLANKSTRING;
      for (int32 i = static_cast<int32>(parameters.find(argument.first)) + 
             static_cast<int32>(argument.first.length() + 1); 
           i < static_cast<int32>(parameters.length()); 
           ++i) {
        if (parameters[i] == '-') { break; }
        tmpString += parameters[i];
      }
      m_flagMap[argument.first] = tmpString;
    }
  }
}

bool
ArgumentParser::addFlag(const String& newFlag) {
  if (m_flagMap.find(newFlag) == m_flagMap.end()) {
    m_flagMap.insert(std::make_pair(newFlag, Utils::BLANKSTRING));
    return true;
  }
  return false;
}

String
ArgumentParser::getFlagValue(const String& flag) {
  if (m_flagMap.find(flag) != m_flagMap.end()) {
    return m_flagMap[flag];
  }
  return Utils::BLANKSTRING;
}

}
