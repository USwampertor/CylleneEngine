#pragma once
#include "cyCorePrerequisites.h"

#include "cyResource.h"
#include <cyColor.h>

namespace CYLLENE_SDK {

/*
 *	@class	TextureMetaData
 *	@brief	The information of the image like bits per pixel, format, size and color
 *
 */
struct CY_CORE_EXPORT TextureMetaData {
public:

  /**
   * Texture format
   */
  uint32 m_textureFormat;

  /**
   * Width of the texture
   */
  uint32 m_width;

  /**
   * Height of the texture
   */
  uint32 m_height;

  /**
   * format of the color
   */
  uint32 m_colorFormat;

  /**
   * bits per pixel
   */
  uint32 m_bpp;
};

class CY_CORE_EXPORT TextureResource : public Resource {

public:

  TextureResource() : Resource(TextureResource::staticType()) {}

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eTEXTURE; 
  }

  virtual void*
  getData() override;

  virtual void
  setData(void* data) override;

  /*
   *	@brief	loads an image from a vector<Color> array of data and image size
   *	@param	const Vector<Color>& the pixels of the image
   *	@param	const uint32& width
   *	@param	const uint32& height
   */
  void
  fromMemory(const Vector<Color>& pixels, 
             const uint32& width, 
             const uint32& height);

public:

  /**
   * The metadata of the image
   */
  TextureMetaData m_metadata;

  /**
   * pixels that make up the image
   */
  Vector<Color> m_pixels;
};

}

