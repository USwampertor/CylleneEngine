#pragma once
#include "cyCorePrerequisites.h"

#include "cyRImage.h"
#include "cyRResource.h"

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

  namespace COLORFORMAT {
    BETTER_ENUM(E, uint32, 
                RGBA_32_FLOAT, // This is right now the default as we convert anything to float[4]
                RGB_32_FLOAT,
                RG_32_FLOAT,
                R_32_FLOAT,
                D_32_FLOAT,
                RGBA_32_UINT,
                RGB_32_UINT,
                RG_32_UINT,
                R_32_UINT,
                D_32_UINT,
                RGBA_32_SINT,
                RGB_32_SINT,
                RG_32_SINT,
                R_32_SINT,
                D_32_SINT,
                RGBA_32_TYPELESS,
                RGB_32_TYPELESS,
                RG_32_TYPELESS,
                R_32_TYPELESS,
                D_32_TYPELESS,
                RGBA_8_FLOAT,
                RGB_8_FLOAT,
                RG_8_FLOAT,
                R_8_FLOAT,
                D_8_FLOAT,
                RGBA_8_UINT,
                RGB_8_UINT,
                RG_8_UINT,
                R_8_UINT,
                D_8_UINT,
                RGBA_8_SINT,
                RGB_8_SINT,
                RG_8_SINT,
                R_8_SINT,
                D_8_SINT,
                RGBA_8_TYPELESS,
                RGB_8_TYPELESS,
                RG_8_TYPELESS,
                R_8_TYPELESS,
                D_8_TYPELESS,
                RGBA_8_UNORM,
                RGB_8_UNORM,
                RG_8_UNORM,
                R_8_UNORM,
                D_8_UNORM);
  }


class CY_CORE_EXPORT RTexture : public RResource {

public:

  RTexture() : RResource(RTexture::staticType()) {}

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
  setImage(const SPtr<RImage>&  img);

  Color
  sample(float u, 
         float v, 
         const TEXTUREMODE::E& mode = TEXTUREMODE::E::eCLAMP,
         const SAMPLERFILTER::E& sampler = SAMPLERFILTER::E::ePOINT);

  Color
  sample(Vector2f uv, 
         const TEXTUREMODE::E& mode = TEXTUREMODE::E::eCLAMP,
         const SAMPLERFILTER::E& sampler = SAMPLERFILTER::E::ePOINT);

  void 
  draw(SPtr<RImage>& img,
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
  SPtr<RImage> m_img;
};

}

