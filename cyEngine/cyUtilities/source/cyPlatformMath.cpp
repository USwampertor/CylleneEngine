/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyPlatformMath.cpp
 * @author Marco "Swampy" Millan
 * @date 2018/09/23 2018
 * @brief Platform member definition to runtime define some variables
 *
 */
 /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#include "cyPlatformMath.h"

namespace CYLLENE_SDK {
  const float PlatformMath::PI = atan(1.0f) * 4.0f;

  const float PlatformMath::HALF_PI = PI / 2.0f;

  const float PlatformMath::DEGREE = 180.0f / PlatformMath::PI;

  const float PlatformMath::RADIAN = PlatformMath::PI / 180.0f;

  const float PlatformMath::EULER = pow((1.0f + (1.0f / 200000.0f)), 200000.0f); //2.71828182f;
  //nauPlatformMath::pow((1.0f + (1.0f / 10000000000000.0f)), 10000000000000.0f);

  const float PlatformMath::KINDASMALLNUMBER = 0.00001f;

  const float PlatformMath::SMALLNUMBER = 0.0000001f;

  const float PlatformMath::EPSILONF = std::numeric_limits<float>::epsilon();

  const float PlatformMath::EPSILOND = std::numeric_limits<double>::epsilon();
}