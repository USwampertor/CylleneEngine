#include "cyShader.h"
#include <cyJSON.h>
#include <cyLogger.h>

namespace CYLLENE_SDK {

void*
ShaderResource::getData() {
  return static_cast<void*>(&m_data);
}

void
ShaderResource::setData(void* data) {
  JSONDocument d;
  String dataString = *reinterpret_cast<String*>(data);
  d.Parse(dataString);
  if (d.HasParseError()) {
    Logger::instance().logError("The shader resource is not a valid JSON");
    return;
  }
  String type = d["type"].GetString();
  if (type != "shader") {
    Logger::instance().logError("The shader resource is not a shader");
    return;
  }

  m_data = d["data"].GetString();
  m_isBlob = d["isBlob"].GetBool();

}

}
