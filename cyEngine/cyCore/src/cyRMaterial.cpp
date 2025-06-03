#include "cyRMaterial.h"

namespace CYLLENE_SDK {

void*
RMaterial::getData() {
  return nullptr;
}

void
RMaterial::setData(void* data) {

}

void
RMaterial::setShader(const SPtr<RShader>& newShader) {
  if (newShader == nullptr) {
    return;
  }

  m_shader = newShader;
}

void
RMaterial::setTexture(TEXTURE_CHANNEL::E name, const SPtr<RTexture>& texture) {
  if (texture == nullptr) {
    return;
  }
  m_textures[name] = texture;
}


}
