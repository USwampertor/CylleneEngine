#include "cyImage.h"
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
ImageResource::getData() {
  return nullptr;
}

void
ImageResource::setData(void* data) {

}

void
ImageResource::create(const uint32& width, const uint32& height, const uint32& bpp) {
  m_metadata.m_width = width;
  m_metadata.m_height = height;
  m_metadata.m_bpp = bpp;
  m_pixels.clear();
  m_pixels.resize(width * height, Color::CLEAR);
}

void
ImageResource::bitBlt(const SharedPointer<ImageResource>& srcImg, 
                      const Rect& srcRect, 
                      const Rect& dstRect, 
                      const TEXTUREMODE::E& format) {
  for (uint32 desty = 0; desty < dstRect.height; ++desty) {
    for (uint32 destx = 0; destx < dstRect.width; ++destx) {

      uint32 srcx = 0;
      uint32 srcy = 0;
      uint32 desx = 0;
      uint32 desy = 0;

      desx = dstRect.x + destx;
      desy = dstRect.y + desty;

      if (desx < 0 || desx >= m_metadata.m_width || desy < 0 || desy >= m_metadata.m_height) {
        continue;
      }

      if (TEXTUREMODE::E::eNONE == format) {
        srcx = srcRect.x + destx;
        srcy = srcRect.y + desty;
      }
      else if (TEXTUREMODE::E::eWRAP == format) {
        srcx = (srcRect.x + destx % srcImg->m_metadata.m_width + srcRect.width) % 
               (srcRect.width);
        srcy = (srcRect.y + desty % srcImg->m_metadata.m_height + srcRect.height) % 
               (srcRect.height);
      }
      else if (TEXTUREMODE::E::eCLAMP == format) {
        srcx = Math::clamp(srcRect.x + destx, uint32(0), 
                           static_cast<uint32>(srcImg->m_metadata.m_width - 1));
        srcy = Math::clamp(srcRect.y + desty, uint32(0), 
                           static_cast<uint32>(srcImg->m_metadata.m_height - 1));
      }
      else if (TEXTUREMODE::E::eMIRROR == format) {
        srcx = mirrorCoord(srcRect.x + destx, srcImg->m_metadata.m_width);
        srcy = mirrorCoord(srcRect.y + desty, srcImg->m_metadata.m_height);
      }
      else if (TEXTUREMODE::E::eSTRETCH == format) {
        float u = static_cast<float>(destx) / dstRect.width;
        float v = static_cast<float>(desty) / dstRect.height;

        srcx = Math::clamp(srcRect.x + static_cast<uint32>(u * srcRect.width - 1), uint32(0), 
                           srcImg->m_metadata.m_width - 1);
        srcy = Math::clamp(srcRect.y + static_cast<uint32>(v * srcRect.height - 1), uint32(0), 
                           srcImg->m_metadata.m_height - 1);
      }
      int32_t pos = (srcy * srcRect.width) + (srcx);
      Pixel srcPixel = srcImg->m_pixels[pos];
      m_pixels[(desy * m_metadata.m_width) + (desx)] = srcPixel;
    }
  }
}

void
ImageResource::clear(const Color& color) {
  std::fill(m_pixels.begin(), m_pixels.end(), color);
}

Pixel&
ImageResource::getPixel(const uint32& x, const uint32& y) {
  return m_pixels[(y * m_metadata.m_width) + x];
}

Pixel&
ImageResource::getPixel(const Vector2i& pos) {
  return getPixel(pos.x, pos.y);
}


void
ImageResource::setPixel(const uint32& x, const uint32& y, const Pixel& color /*= Pixel::CLEAR */) {
  m_pixels[(y * m_metadata.m_width) + x] = color;
}

void
ImageResource::setPixel(const Vector2i& pos, const Pixel& color /*= Pixel::CLEAR */) {
  setPixel(pos.x, pos.y, color);
}

void
ImageResource::filterPixel(const Rect& rect, const Pixel& color, const uint32& tolerance) {
  for (uint32 desty = 0; desty < rect.height; ++desty) {
    for (uint32 destx = 0; destx < rect.width; ++destx) {

      uint32 srcx = 0;
      uint32 srcy = 0;

      srcx = rect.x + destx;
      srcy = rect.y + desty;

      Pixel srcPixel = m_pixels[(srcy * rect.width) + (srcx)];
      if (srcPixel == color) {
        m_pixels[(srcy * rect.width) + (srcx)] = Color::CLEAR;
      }
    }
  }
}

void
ImageResource::fromMemory(const Vector<Pixel>& pixels, const uint32& width, const uint32& height) {
  m_pixels = pixels;
  m_metadata.m_width;
  m_metadata.m_height;
}


}