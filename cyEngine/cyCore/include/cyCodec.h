/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	cyCodec.h
 * @author 	Marco "Swampy" Millan
 * @date 	2022/03/03
 * @brief 	
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCorePrerequisites.h"

namespace CYLLENE_SDK {

  
class CY_CORE_EXPORT Codec
{
public:
  static bool
  isAcceptedFormat(const String& type) {
    return std::find(m_acceptedFormats.begin(), m_acceptedFormats.end(), type) != 
                     m_acceptedFormats.end();
  }
  
  static const Vector<String> m_acceptedFormats;

};
}
