#pragma once
#include "cyCorePrerequisites.h"

#include "cyResource.h"
#include <cyColor.h>

namespace CYLLENE_SDK {

struct CY_CORE_EXPORT TextureMetaData {
public:
  uint32 m_textureFormat;
  uint32 m_width;
  uint32 m_height;
  uint32 m_colorFormat;
  uint32 m_bpp;
};

class CY_CORE_EXPORT TextureResource : public Resource {

public:

  TextureResource() : Resource(TextureResource::staticType()) {}

  TextureResource(const Path& newFile)
    : Resource(newFile) {
    m_type = TextureResource::staticType();
  }

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eTEXTURE; 
  }

  virtual void
  fromMemory(void* pixels, uint32 width, uint32 height);

  virtual void
  setData(void* data) override {
    auto tupleData = static_cast<std::tuple<TextureMetaData, const Color*>*>(data);
    m_metadata = std::get<0>(*tupleData);

    for (uint32 i = 0; i < m_metadata.m_width * m_metadata.m_height; ++i) {
      m_pixels.push_back(std::get<1>(*tupleData)[i]);
    }

  }

public:
  TextureMetaData m_metadata;
  Vector<Color> m_pixels;
};


}

