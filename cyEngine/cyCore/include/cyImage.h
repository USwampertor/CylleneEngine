/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	cyImage.h
 * @author 	Marco "Swampy" Millan
 * @date 	2022/09/30
 * @brief 	
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCorePrerequisites.h"}
#include "cyResource.h"

#include <cyColor.h>
#include <cyVector4f.h>

namespace CYLLENE_SDK {

// template<typename T>
// struct Pixel
// {
// 
//   union {
//     struct {
//       T r;
//       T g;
//       T b;
//       T a;
//     };
// 
//     T _m[4];
//   };
// };


  namespace TEXTUREMODE
  {
  BETTER_ENUM(E, 
              uint32, 
              eUNKNOWN, 
              eNONE, 
              eREPEAT, 
              eCLAMP, 
              eMIRROR, 
              eSTRETCH,
              eWRAP);
  }

  namespace IMAGEFORMAT
  {
  BETTER_ENUM(E,
              uint32,
              eUNKNOWN,
              eRAW,
              eBMP);

  }

  namespace BLENDMODE
  {
  BETTER_ENUM(E,
              uint32,
              eNONE,
              eALPHABLEND,
              eADDITIVE);
  }

using Pixel = Color;

struct ImageMetadata
{
public:

  ImageMetadata() = default;

  ImageMetadata(const uint32& width, 
                const uint32& height, 
                const uint32& bpp = 32,         // RGBA
                const uint32& fileformat = 34,  // IMGEXT::E::RAW
                const uint32& colortype = 4)    // FIC_RGBALPHA    
    : m_format(fileformat),
      m_width(width),
      m_height(height),
      m_colortype(colortype),
      m_bpp(bpp) {}

  uint32 m_format;
  uint32 m_width;
  uint32 m_height;
  uint32 m_colortype;
  uint32 m_bpp;
};

struct Rect
{
public:

  Rect() = default;

  Rect(uint32 nx, uint32 ny, uint32 nwidth, uint32 nheight)
    : x(nx),
      y(ny),
      width(nwidth),
      height(nheight) {}

  uint32 x;
  uint32 y;
  uint32 width;
  uint32 height;
};

class CY_CORE_EXPORT ImageResource : public Resource
{
public:

  ImageResource() : Resource(ImageResource::staticType()) {}

  ImageResource(uint32 width, uint32 height, Vector<Pixel> vector, uint32 bpp = 32)
    : Resource(ImageResource::staticType()),
      m_metadata(width, height, bpp),
      m_pixels(vector) {}

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eIMAGE;
  }

  virtual void*
  getData() override;

  virtual void
  setData(void* data) override;

  const uint32& getWidth() {
    return m_metadata.m_width;
  }

  const uint32& getHeight() {
    return m_metadata.m_height;
  }

  const uint32& getBPP() {
    return m_metadata.m_bpp;
  }

  void
  create(const uint32& width, const uint32& height, const uint32& bpp);

  void
  bitBlt(const SPtr<ImageResource>& srcImg,
         const Rect& srcRect, 
         const Rect& dstRect, 
         const TEXTUREMODE::E& format = TEXTUREMODE::E::eNONE);

  void
  clear(const Color& color = Color::CLEAR);

  Pixel&
  getPixel(const uint32& x, const uint32& y);

  Pixel&
  getPixel(const Vector2i& pos);

  void
  setPixel(const uint32& x, const uint32& y, const Pixel& color = Pixel::CLEAR);

  void
  setPixel(const Vector2i& pos, const Pixel& color = Pixel::CLEAR);

  void
  filterPixel(const Rect& rect, const Pixel& color = Pixel::WHITE, const uint32& tolerance = 0);

  void
  fromMemory(const Vector<Pixel>& pixels, const uint32& width, const uint32& height);

  ImageMetadata m_metadata;

  Vector<Pixel> m_pixels;
  // union {
  //   float Vector<Vector<Pixel<T>>> m_m;
  //   float Vector<Pixel<T>> m_v;
  // };

};



}


