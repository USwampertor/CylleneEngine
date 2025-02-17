#pragma once
#include "cyCorePrerequisites.h"

#include "cyImage.h"
#include "cyResource.h"

#include <cyColor.h>
#include <cyVector2f.h>

namespace CYLLENE_SDK {

/*
 *	@class	TextureMetaData
 *	@brief	The information of the image like bits per pixel, format, size and color
 *
 */
// struct CY_CORE_EXPORT TextureMetaData {
// public:
// 
//   /**
//    * Texture format
//    */
//   uint32 m_textureFormat;
// 
//   /**
//    * Width of the texture
//    */
//   uint32 m_width;
// 
//   /**
//    * Height of the texture
//    */
//   uint32 m_height;
// 
//   /**
//    * format of the color
//    */
//   uint32 m_colorFormat;
// 
//   /**
//    * bits per pixel
//    */
//   uint32 m_bpp;
// };


  namespace SAMPLERFILTER {
  BETTER_ENUM(E,
              uint32,
              ePOINT,
              eLINEAR,
              eBILINEAL,
              eTRILINEAL);
  }


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

  void
  adjustTextureAddress(float& u,
                       float& v,
                       const TEXTUREMODE::E& mode = TEXTUREMODE::E::eCLAMP);
  /*
   *	@brief	loads an image from a vector<Color> array of data and image size
   *	@param	const Vector<Color>& the pixels of the image
   *	@param	const uint32& width
   *	@param	const uint32& height
   */
//   void
//   fromMemory(const Vector<Color>& pixels, 
//              const uint32& width, 
//              const uint32& height);

  void
  setImage(const SharedPointer<ImageResource>&  img);

  Color
  sample(float& u, 
         float& v, 
         const TEXTUREMODE::E& mode = TEXTUREMODE::E::eCLAMP,
         const SAMPLERFILTER::E& sampler = SAMPLERFILTER::E::ePOINT);

  Color
  sample(Vector2f& uv, 
         const TEXTUREMODE::E& mode = TEXTUREMODE::E::eCLAMP,
         const SAMPLERFILTER::E& sampler = SAMPLERFILTER::E::ePOINT);

  void 
  draw(SharedPointer<ImageResource>& img,
       int32_t x,
       int32_t y,
       const Rect& srcRect, 
       const TEXTUREMODE::E& mode = TEXTUREMODE::E::eCLAMP,
       const BLENDMODE::E& blend = BLENDMODE::E::eALPHABLEND,
       const SAMPLERFILTER::E& filter = SAMPLERFILTER::E::ePOINT);

  void
  createMipMaps(const uint32 levels = 1);

public:

  /**
   * The metadata of the image
   */
  // TextureMetaData m_metadata;

  /**
   * pixels that make up the image
   */
  // Vector<Color> m_pixels;

  /**
   * The image resource being used
   */
  SharedPointer<ImageResource> m_img;
};

}

