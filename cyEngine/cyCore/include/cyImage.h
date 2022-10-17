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

#include "cyCorePrerequisites.h"
#include <cyColor.h>
#include <cyVector4f.h>

namespace CYLLENE_SDK {

template<typename T>
struct Pixel
{

  union {
    struct {
      T r;
      T g;
      T b;
      T a;
    };

    T _m[4];
  };
};


template<typename T>
class CY_CORE_EXPORT Image
{
public:

  Image() = default;

  Image(uint32 width, uint32 height, Vector<Pixel<T>> vector)
    : m_width(width),
      m_height(height),
      m_v(vector) {}

  uint32 m_width;
  uint32 m_height;

  union {
    float Vector<Vector<Pixel<T>>> m_m;
    float Vector<Pixel<T>> m_v;
  };

};



}


