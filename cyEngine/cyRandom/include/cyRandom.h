/**
 * @file cyRandom.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains a Random Number Generator class that internally uses different systems
 */
#pragma once

#include <chrono>


#include "cyRandomPrerequisites.h"
#include "cyZiggurat.h"
#include "cyMT.h"

#include <cyVector2f.h>
#include <cyVector2i.h>
#include <cyVector3f.h>
#include <cyVector4f.h>
#include <cyColor.h>

namespace CYLLENE_SDK {

  namespace RANDOM_METHOD
  {
  BETTER_ENUM(E, uint32,
              eZIGGURAT,
              eMT,
              eBBS,
              eMSM);
  }


/**
 * @struct Random
 * @brief Central random number utilities wrapper. Provides multiple RNG backends
 *        and convenience helpers to get values in different ranges and types.
 */
struct CY_RANDOM_EXPORT Random
{
public:

  /**
   * @brief Initializes the RNG subsystem and seeds internal generators.
   *        Must be called before using other Random functions.
   */
  static void 
  init() {
    m_method = 0;
    std::mt19937 random(0x1976F3A4);
    set(random());
    m_ziggurat.r4_nor_setup(m_ziggurat.m_kn, m_ziggurat.m_fn, m_ziggurat.m_wn);
  }

  /**
   * @brief Returns a random value converted to type T from the currently selected RNG method.
   * @tparam T Return type to convert the generated value to.
   * @return Random value cast to T.
   */
  template<typename T>
  static T
  get() {

    float value = 0.0f;

    RANDOM_METHOD::E method = RANDOM_METHOD::E::_from_integral(m_method);

    switch (method) {
    case RANDOM_METHOD::E::eZIGGURAT:
      value = m_ziggurat.next();
      break;
    case RANDOM_METHOD::E::eMT:
      value = m_mt.next();
      break;
    case RANDOM_METHOD::E::eBBS:
      break;
    case RANDOM_METHOD::E::eMSM:
      break;
    }

    return static_cast<T>(value);
  }

  /**
   * @brief Returns a random value in the inclusive range [min, max].
   * @tparam T Type of the range values.
   * @param min Minimum value (inclusive).
   * @param max Maximum value (inclusive).
   * @return Random value between min and max.
   */
  template<typename T>
  static T
  getRanged(const T& min, const T& max) {
    CY_ASSERT(max > min);
    const float range = static_cast<float>(max - min);
    constexpr static float minimum = 0e-5f;
    float value = getNormalized() * (static_cast<float>(range) - minimum);
    return min + static_cast<T>(value);
  }

  /**
   * @brief Roll a number of dice and sum the results.
   * @param sides Number of sides per die (default 6).
   * @param count Number of dice to roll (default 1).
   * @return Sum of all dice rolls.
   */
  static uint32
  rollDice(const uint32 sides = 6, const uint32 count = 1) {
    CY_ASSERT(sides > 1 && count > 0);
    uint32 result = 0;
    for (uint32 i = 0; i < count; ++i) {
      result += getRanged<uint32>(1, sides);
    }
    return result;
  }

  /**
   * @brief Return a random float in range [min, max].
   * @param min Minimum value (inclusive).
   * @param max Maximum value (inclusive).
   * @return Random float between min and max.
   */
  static float
  getRangeFloat(const float& min, const float& max);

  /**
   * @brief Return a random int32 in range [min, max].
   * @param min Minimum value (inclusive).
   * @param max Maximum value (inclusive).
   * @return Random int32 between min and max.
   */
  static int32
  getRangeInt32(const int32& min, const int32& max);

  /**
   * @brief Return a random uint32 in range [min, max].
   * @param min Minimum value (inclusive).
   * @param max Maximum value (inclusive).
   * @return Random uint32 between min and max.
   */
  static uint32
  getRangeUint32(const uint32& min, const uint32& max);

  /**
   * @brief Seed the random subsystem with a new seed value.
   * @param newSeed New seed to use.
   */
  static void
  set(const uint32& newSeed);

  /**
   * @brief Returns a normalized random float in range [0,1).
   * @return Normalized float.
   */
  static float
  getNormalized();

  /**
   * @brief Returns a random Vector2i with components in [-1,1] or similar normalized range.
   * @return Random Vector2i.
   */
  static Vector2i
  getNormVector2i();

  /**
   * @brief Returns a random Vector2f with components in [-1,1] or similar normalized range.
   * @return Random Vector2f.
   */
  static Vector2f
  getNormVector2f();

  /**
   * @brief Returns a random Vector3f with components in [-1,1] or similar normalized range.
   * @return Random Vector3f.
   */
  static Vector3f
  getNormVector3f();

  /**
   * @brief Returns a random Vector4f with components in [-1,1] or similar normalized range.
   * @return Random Vector4f.
   */
  static Vector4f
  getNormVector4f();

  /**
   * @brief Returns a random color.
   * @return Random Color.
   */
  static Color
  getColor();

  /**
   * @brief Returns a Vector2i scaled by thickness parameter.
   * @param thickness Scale / magnitude of returned vector.
   * @return Vector2i with randomized components.
   */
  static Vector2i
  getVector2i(const float& thickness);

  /**
   * @brief Returns a Vector2f scaled by thickness parameter.
   * @param thickness Scale / magnitude of returned vector.
   * @return Vector2f with randomized components.
   */
  static Vector2f
  getVector2f(const float& thickness);

  /**
   * @brief Returns a Vector3f scaled by thickness parameter.
   * @param thickness Scale / magnitude of returned vector.
   * @return Vector3f with randomized components.
   */
  static Vector3f
  getVector3f(const float& thickness);

  /**
   * @brief Returns a Vector4f scaled by thickness parameter.
   * @param thickness Scale / magnitude of returned vector.
   * @return Vector4f with randomized components.
   */
  static Vector4f
  getVector4f(const float& thickness);

  /**
   * @brief Change the active random generation method.
   * @param method RNG backend to use (defaults to eZIGGURAT).
   */
  static void
  changeMethod(const RANDOM_METHOD::E& method = RANDOM_METHOD::E::eZIGGURAT);

public:

  /** @brief Internal seed state (implementation-specific). */
  static uint32 m_seed[4];

  /** @brief Ziggurat RNG instance used when eZIGGURAT method is selected. */
  static Ziggurat m_ziggurat;

  /** @brief Mersenne Twister wrapper used when eMT method is selected. */
  static MT m_mt;

  /** @brief Currently selected RNG method (index into RANDOM_METHOD). */
  static uint32 m_method;
};

}
