#include "cyTexture.h"


namespace CYLLENE_SDK {
void
TextureResource::fromMemory(const Vector<Color>& pixels, 
                            const uint32& width, 
                            const uint32& height) {

}

void*
TextureResource::getData() {
  std::tuple<TextureMetaData, Vector<Color>>* tupleData = 
    new std::tuple<TextureMetaData, Vector<Color>>(m_metadata, 
                                                   m_pixels);
  return reinterpret_cast<void*>(tupleData);
}

void
TextureResource::setData(void* data) {
  auto* tupleData = static_cast<std::tuple<TextureMetaData, Vector<Color>>*>(data);
  m_metadata = std::get<0>(*tupleData);
  m_pixels = std::get<1>(*tupleData);
  // ??? Should this be done?
  delete data;
}


}