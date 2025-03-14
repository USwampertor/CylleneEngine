/*********************************************/
/*
 * @file 	cyRandom
 * @author	Marco "Swampertor" Millan
 * @date	27/09/2021
 * @brief	
 *
 */
/******************************************** */

#include "cyRandom.h"

namespace CYLLENE_SDK {

uint32 Random::m_seed[4] = { 0,0,0,0 };

uint32 Random::m_method = 0;

Ziggurat Random::m_ziggurat = {};

void
Random::set(const uint32& newSeed) {
//     m_seed[0] = newSeed;
//     m_seed[1] = newSeed * 0xA89F234B + 1; // Arbitrary random numbers
//     m_seed[2] = newSeed * 0x81D1A5F4 + 1;
//     m_seed[3] = newSeed * 0x912FF1AD + 1;

  m_ziggurat.m_generator = { newSeed };
}

float
Random::getRangeFloat(const float& min, const float& max) {
  CY_ASSERT(max > min);
  const float range = max - min + 1;
  constexpr static float minimum = 0e-5f;
  return min + static_cast<float>(getNormalized() * (static_cast<float>(range) - minimum));
}

int32
Random::getRangeInt32(const int32& min, const int32& max) {
  CY_ASSERT(max > min);
  const int32 range = max - min + 1;
  constexpr static float minimum = 0e-5f;
  return min + static_cast<int32>(getNormalized() * (static_cast<float>(range) - minimum));
}

uint32
Random::getRangeUint32(const uint32& min, const uint32& max) {
  CY_ASSERT(max > min);
  constexpr static float minimum = 0e-5f;
  return static_cast<uint32>(getNormalized() * (static_cast<float>(max + 1)));
}

float
Random::getNormalized() {
  // TODO: Simplify this
  float f = 0.0f;
  uint32 seed = 0;
  RANDOM_METHOD::E method = RANDOM_METHOD::E::_from_integral(m_method);

  switch (method) {
  case +RANDOM_METHOD::E::eZIGGURAT:
    seed = m_ziggurat.m_generator();
    f = m_ziggurat.r4_uni(seed);
    break;
  case +RANDOM_METHOD::E::eMT:
    break;
  case +RANDOM_METHOD::E::eBBS:
    break;
  case +RANDOM_METHOD::E::eMSM:
    break;
  }

  return f;
}

Vector2i
Random::getNormVector2i() {
  Vector2i output;
  float sqrdSize;

  do {
    output.x = getRangeInt32(-1, 1);
    output.y = getRangeInt32(-1, 1);
    sqrdSize = static_cast<float>(output.sqrMagnitude());
  } while (sqrdSize > 1.0f || sqrdSize < 0.001f);

  output.normalize();
  return output;
}

Vector2f
Random::getNormVector2f() {
  Vector2f output;
  float sqrdSize;

  do {
    output.x = getRangeFloat(-1.0f, 1.0f);
    output.y = getRangeFloat(-1.0f, 1.0f);
    sqrdSize = output.sqrMagnitude();
  } while (sqrdSize > 1.0f || sqrdSize < 0.001f);

  output.normalize();
  return output;
}

Vector3f
Random::getNormVector3f() {
  Vector3f output;
  float sqrdSize;

  do {
    output.x = getRangeFloat(-1.0f, 1.0f);
    output.y = getRangeFloat(-1.0f, 1.0f);
    output.z = getRangeFloat(-1.0f, 1.0f);
    sqrdSize = output.sqrMagnitude();
  } while (sqrdSize > 1.0f || sqrdSize < 0.001f);

  output.normalize();
  return output;
}

Vector2i
Random::getVector2i(const float& thickness) {
  return getNormVector2i() * static_cast<int32>(thickness);
}

Vector2f
Random::getVector2f(const float& thickness) {
  return getNormVector2f() * thickness;

}

Vector3f
Random::getVector3f(const float& thickness) {
  return getNormVector3f() * thickness;

}

void
Random::changeMethod(const RANDOM_METHOD::E& method /* = 0 */) {
  m_method = method._to_integral();
}
}

