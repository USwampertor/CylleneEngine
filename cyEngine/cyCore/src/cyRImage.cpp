#include "cyRImage.h"
#include <cyMath.h>
#include <cyVector2i.h>

namespace CYLLENE_SDK {

uint32
mirrorCoord(int32 coord, int32 max) {
  if (max <= 0) return 0;

  coord = coord % (2 * max);
  if (coord < 0) coord += 2 * max;

  return coord >= max ? 2 * max - coord - 1 : coord;
}

void*
RImage::getData() {
  std::tuple<ImageMetadata, Vector<Color>>* tupleData = 
    new std::tuple<ImageMetadata, Vector<Color>>(m_metadata, 
                                                   m_pixels);
  return reinterpret_cast<void*>(tupleData);
}

void
RImage::setData(void* data) {
  auto* tupleData = static_cast<std::tuple<ImageMetadata, Vector<Color>>*>(data);
  m_metadata = std::get<0>(*tupleData);
  m_pixels = std::get<1>(*tupleData);
  // ??? Should this be done?
  delete data;
}

void
RImage::create(const uint32& width, const uint32& height, const uint32& bpp) {
  m_metadata.m_width = width;
  m_metadata.m_height = height;
  m_metadata.m_bpp = bpp;
  m_pixels.clear();
  m_pixels.resize(width * height, Color::CLEAR);
}

void
RImage::bitBlt(const SPtr<RImage>& srcImg, 
                      const Rect& srcRect, 
                      const Rect& dstRect, 
                      const TEXTUREMODE::E& format) {
  for (uint32 desty = 0; desty < dstRect.getHeight(); ++desty) {
    for (uint32 destx = 0; destx < dstRect.getWidth(); ++destx) {

      uint32 srcx = 0;
      uint32 srcy = 0;
      uint32 desx = 0;
      uint32 desy = 0;

      desx = dstRect.getX() + destx;
      desy = dstRect.getY() + desty;

      if (desx < 0 || desx >= m_metadata.m_width || desy < 0 || desy >= m_metadata.m_height) {
        continue;
      }

      if (+TEXTUREMODE::E::eNONE == format) {
        srcx = srcRect.getX() + destx;
        srcy = srcRect.getY() + desty;
      }
      else if (+TEXTUREMODE::E::eWRAP == format  || +TEXTUREMODE::E::eREPEAT == format) {
        srcx = (srcRect.getX() + destx % srcImg->m_metadata.m_width + srcRect.getWidth()) %
               (srcRect.getWidth());
        srcy = (srcRect.getY() + desty % srcImg->m_metadata.m_height + srcRect.getHeight()) %
               (srcRect.getHeight());
      }
      else if (+TEXTUREMODE::E::eCLAMP == format) {
        srcx = Math::clamp(srcRect.getX() + destx, uint32(0),
                           static_cast<uint32>(srcImg->m_metadata.m_width - 1));
        srcy = Math::clamp(srcRect.getY() + desty, uint32(0),
                           static_cast<uint32>(srcImg->m_metadata.m_height - 1));
      }
      else if (+TEXTUREMODE::E::eMIRROR == format) {
        srcx = mirrorCoord(srcRect.getX() + destx, srcImg->m_metadata.m_width);
        srcy = mirrorCoord(srcRect.getY() + desty, srcImg->m_metadata.m_height);
      }
      else if (+TEXTUREMODE::E::eSTRETCH == format) {
        float u = static_cast<float>(destx) / dstRect.getWidth();
        float v = static_cast<float>(desty) / dstRect.getHeight();

        srcx = Math::clamp(srcRect.getX() + static_cast<uint32>(u * srcRect.getWidth() - 1), uint32(0),
                           srcImg->m_metadata.m_width - 1);
        srcy = Math::clamp(srcRect.getY() + static_cast<uint32>(v * srcRect.getHeight() - 1), uint32(0),
                           srcImg->m_metadata.m_height - 1);
      }
      int32_t pos = (srcy * srcRect.getWidth()) + (srcx);
      Pixel srcPixel = srcImg->m_pixels[pos];
      m_pixels[(desy * m_metadata.m_width) + (desx)] = srcPixel;
    }
  }
}

void
RImage::clear(const Color& color) {
  std::fill(m_pixels.begin(), m_pixels.end(), color);
}

Pixel&
RImage::getPixel(const uint32& x, const uint32& y) {
  return m_pixels[(y * m_metadata.m_width) + x];
}

Pixel&
RImage::getPixel(const Vector2i& pos) {
  return getPixel(pos.x, pos.y);
}


void
RImage::setPixel(const uint32& x, const uint32& y, const Pixel& color /*= Pixel::CLEAR */) {
  m_pixels[(y * m_metadata.m_width) + x] = color;
}

void
RImage::setPixel(const Vector2i& pos, const Pixel& color /*= Pixel::CLEAR */) {
  setPixel(pos.x, pos.y, color);
}

void
RImage::filterPixel(const Rect& rect, const Pixel& color, const uint32& tolerance) {
  for (uint32 desty = 0; desty < rect.getHeight(); ++desty) {
    for (uint32 destx = 0; destx < rect.getWidth(); ++destx) {

      uint32 srcx = 0;
      uint32 srcy = 0;

      srcx = rect.getX() + destx;
      srcy = rect.getY() + desty;

      Pixel srcPixel = m_pixels[(srcy * rect.getWidth()) + (srcx)];
      if (srcPixel == color) {
        m_pixels[(srcy * rect.getWidth()) + (srcx)] = Color::CLEAR;
      }
    }
  }
}

void
RImage::fromMemory(const Vector<Pixel>& pixels, const uint32& width, const uint32& height) {
  m_pixels = pixels;
  m_metadata.m_width;
  m_metadata.m_height;
}


}