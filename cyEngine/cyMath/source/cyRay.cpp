#include "cyRay.h"
#include <cyUtilities.h>


namespace CYLLENE_SDK
{
String
Ray::toString() {
  return Utils::format("Origin: %s, Direction: %s", m_origin.toString().c_str(), m_direction.toString().c_str());
}

}