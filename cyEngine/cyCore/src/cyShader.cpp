#include "cyShader.h"

namespace CYLLENE_SDK {

void*
ShaderResource::getData() {
  return static_cast<void*>(&m_data);
}

void
ShaderResource::setData(void* data) {
  String& s = *(static_cast<String*>(data));
  m_data = s;
}

}
