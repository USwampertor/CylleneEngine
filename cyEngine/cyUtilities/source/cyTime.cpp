/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyTime.cpp
 * @author Marco "Swampy" Millan
 * @date 8/6/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#include "cyTime.h"

namespace CYLLENE_SDK
{
  void 
  Time::init() {
    m_timer = HighClock::now();
  }

  Date 
  Time::now() {
    TimeType tt = SystemClock::to_time_t(SystemClock::now());
    Date localTime = *std::localtime(&tt);
    return localTime;
  }

  Date
  Time::nowUTC() {
    TimeType tt = SystemClock::to_time_t(SystemClock::now());
    Date gmTime = *std::gmtime(&tt);
    return gmTime;
  }

  void
  Time::update() {
    auto now = HighClock::now();
    Microseconds ms = std::chrono::duration_cast<Microseconds>(now - m_timer);
    m_timer = now;
    m_delta = static_cast<float>(ms.count());
  }

  float
  Time::deltaTime() {
    return m_delta * 0.001f;
  }

  float
  Time::deltaTime(DELTA_TYPE::E& delta) {
    if      (+DELTA_TYPE::E::MILLISECOND == delta)  { return m_delta * 0.001f; }
    else if (+DELTA_TYPE::E::SECOND == delta)       { return m_delta * 0.000001f; }
    return m_delta;
  }
}
