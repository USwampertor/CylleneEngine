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

  void
  Random::set(uint32 newSeed) {
    m_seed[0] = newSeed;
    m_seed[1] = newSeed * 0xA89F234B + 1; // Arbitrary random numbers
    m_seed[2] = newSeed * 0x81D1A5F4 + 1;
    m_seed[3] = newSeed * 0x912FF1AD + 1;
  }

  uint32
  Random::get() {
    /* Algorithm "xor128" from p. 5 of Marsaglia, "Xorshift RNGs" */
    uint32 t = m_seed[3];

    uint32 s = m_seed[0];  /* Perform a contrived 32-bit shift. */
    m_seed[3] = m_seed[2];
    m_seed[2] = m_seed[1];
    m_seed[1] = s;

    t ^= t << 11;
    t ^= t >> 8;
    m_seed[0] = t ^ s ^ (s >> 19);
    return m_seed[0];
  }

  float
  Random::getRangeFloat(float min, float max) {
    CY_ASSERT(max > min);
    const float range = max - min + 1;
    constexpr static float minimum = 0e-5f;
    return min + static_cast<float>(getNormalized() * (static_cast<float>(range) - minimum));
  }

  int32
  Random::getRangeInt32(int32 min, int32 max) {
    CY_ASSERT(max > min);
    const int32 range = max - min + 1;
    constexpr static float minimum = 0e-5f;
    return min + static_cast<int32>(getNormalized() * (static_cast<float>(range) - minimum));
  }

  uint32
  Random::getRangeUint32(uint32 max) {
    uint32 min = 0;
    CY_ASSERT(max > min);
    constexpr static float minimum = 0e-5f;
    return static_cast<uint32>(getNormalized() * (static_cast<float>(max + 1)));
  }

  float
  Random::getNormalized() {
    float f = static_cast<float>(get() & 0x007FFFFF) / 8388607.0f;
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
  Random::getVector2i(float thickness) {
    return Vector2i();
  }

  Vector2f
  Random::getVector2f(float thickness) {
    return Vector2f();

  }

  Vector3f
  Random::getVector3f(float thickness) {
    return Vector3f();

  }
}

