#include "cyRTexture.h"
#include "cyMath.h"

namespace CYLLENE_SDK {

void*
RTexture::getData() {
//   std::tuple<TextureMetaData, Vector<Color>>* tupleData = 
//     new std::tuple<TextureMetaData, Vector<Color>>(m_metadata, 
//                                                    m_pixels);
//   return reinterpret_cast<void*>(tupleData);
  return m_img->getData();
}

void
RTexture::setData(void* data) {
//   auto* tupleData = static_cast<std::tuple<TextureMetaData, Vector<Color>>*>(data);
//   m_metadata = std::get<0>(*tupleData);
//   m_pixels = std::get<1>(*tupleData);
//   // ??? Should this be done?
//   delete data;
}

void
RTexture::adjustTextureAddress(float& u, 
                               float& v, 
                               const TEXTUREMODE::E& mode /* = TEXTUREMODE::E::eCLAMP */) {
  if (TEXTUREMODE::E::eWRAP == mode || TEXTUREMODE::E::eREPEAT == mode) {
    u = Math::mod(u, 1.0f);
    v = Math::mod(v, 1.0f);
  }
  else if (TEXTUREMODE::E::eMIRROR == mode) {
    u = Math::mod(u, 2.0f);
    v = Math::mod(v, 2.0f);

    u = u < 0.0f ? 2.0f + u : u > 1.0f ? 2.0f - u : u;
    v = v < 0.0f ? 2.0f + v : v > 1.0f ? 2.0f - v : v;
  }
  else if (TEXTUREMODE::E::eSTRETCH == mode) {

  }
  else if (TEXTUREMODE::E::eCLAMP == mode) {
    u = Math::clamp(u, 0.0f, 1.0f);
    v = Math::clamp(v, 0.0f, 1.0f);
  }
}

void
RTexture::setImage(const SPtr<RImage>& img) {
  m_img = img;
}

Color
RTexture::sample(float u, 
                 float v, 
                 const TEXTUREMODE::E& mode /* = TEXTUREMODE::E::eCLAMP */, 
                 const SAMPLERFILTER::E& sampler /* = SAMPLERFILTER::E::ePOINT */) {
  adjustTextureAddress(u, v, mode);
  float x = u * (m_img->m_metadata.m_width - 1);
  float y = v * (m_img->m_metadata.m_height - 1);
  if (SAMPLERFILTER::E::ePOINT == sampler) {
    // TODO: REMOVE REDUNDANCY

    return m_img->getPixel(x, y);
  }
  else if (SAMPLERFILTER::E::eLINEAR == sampler) {

    int32 x0 = static_cast<int32>(x); // std::clamp(static_cast<uint32_t>(x), uint32_t(0), m_img.m_width - 1);
    int32 y0 = static_cast<int32>(y); // std::clamp(static_cast<uint32_t>(y), uint32_t(0), m_img.m_height - 1);

    int32 x1 = Math::min(x0 + 1, static_cast<int32>(m_img->m_metadata.m_width - 1));
    int32 y1 = Math::min(y0 + 1, static_cast<int32>(m_img->m_metadata.m_height - 1));

    float dx = x - x0;
    float dy = y - y0;

    Color c00(m_img->getPixel(x0, y0));
    Color c10(m_img->getPixel(x1, y0));
    Color c01(m_img->getPixel(x0, y1));
    Color c11(m_img->getPixel(x1, y1));

    Color c0 = c00 * (1.0f - dx) + c10 * dx;
    Color c1 = c01 * (1.0f - dx) + c11 * dx;

    return c0 * (1.0f - dy) + c1 * dy;
  }
  else if (SAMPLERFILTER::E::eBILINEAL == sampler) {

  }

}

Color
RTexture::sample(Vector2f uv, 
                 const TEXTUREMODE::E& mode /* = TEXTUREMODE::E::eCLAMP */, 
                 const SAMPLERFILTER::E& sampler /* = SAMPLERFILTER::E::ePOINT */) {
  return sample(uv.x, uv.y, mode, sampler);
}

void
RTexture::draw(SPtr<RImage>& img,
                      int32_t x,
                      int32_t y,
                      const Rect& srcRect, 
                      const TEXTUREMODE::E& mode /* = TEXTUREMODE::E::eCLAMP */, 
                      const BLENDMODE::E& blend /* = BLENDMODE::E::eALPHABLEND */,
                      const SAMPLERFILTER::E& filter) {
  int32 realWidth = srcRect.width - srcRect.x;
  int32 realHeight = srcRect.height - srcRect.y;

  if (realWidth < 0 || realHeight < 0) {
    return;
  }

  if (x + realWidth > img->m_metadata.m_width) {
    realWidth -= (x + realWidth) - img->m_metadata.m_width;
  }
  if (y + realHeight > img->m_metadata.m_height) {
    realHeight -= (y + realHeight) - img->m_metadata.m_height;
  }

  for (int32_t dsty = 0; dsty < srcRect.height; ++dsty) {
    for (int32_t dstx = 0; dstx < srcRect.width; ++dstx) {

      float u = static_cast<float>(dstx) / srcRect.width;
      float v = static_cast<float>(dsty) / srcRect.height;
      // TODO: Check on samplerFilter
      Color srcColor = sample(u, v, mode);
      Color dstColor(img->getPixel(x + dstx, y + dsty));
      Color blendedColor = Color::CLEAR;

      if (BLENDMODE::E::eALPHABLEND == blend) {
        blendedColor = srcColor * srcColor.a + dstColor * (1.0f - srcColor.a);
      }
      else if (BLENDMODE::E::eADDITIVE == blend) {
        blendedColor = (srcColor * srcColor.a) + dstColor;
        blendedColor.saturate();
      }
      else if (BLENDMODE::E::eNONE == blend) {
        blendedColor = srcColor;
      }
      img->setPixel(x + dstx, y + dsty, blendedColor);
    }
  }
}


}