/**
 * @file cyTime.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Time.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyUtilities.h"
#include "cyModule.h"

namespace CYLLENE_SDK {

  namespace DELTA_TYPE
  {
    BETTER_ENUM(E, uint32, 
                eMICROSECOND = 0,
                eMILLISECOND,
                eSECOND);
  }

/**
 * @struct Date
 * @brief Convenience wrapper around `tm` with formatting helpers.
 */
struct CY_UTILITY_EXPORT Date : public TM
{
public:
  
  Date() = default;

  ~Date() = default;

  /**
   * @brief Builds a Date from a native `tm` struct.
   * @param father Source `tm` data.
   */
  Date(const TM& father) {
    this->tm_hour   = father.tm_hour;
    this->tm_isdst  = father.tm_isdst;
    this->tm_mday   = father.tm_mday;
    this->tm_min    = father.tm_min;
    this->tm_mon    = father.tm_mon;
    this->tm_sec    = father.tm_sec;
    this->tm_wday   = father.tm_wday;
    this->tm_yday   = father.tm_yday;
    this->tm_year   = father.tm_year;
  }

  /**
   * @brief Assigns from a native `tm` struct.
   * @param father Source `tm` data.
   * @return Assigned `Date`.
   */
  Date operator=(const TM& father) {
    return Date(father);
  }

  /**
   * @brief Formats date/time using a strftime format string.
   * @param format Strftime format.
   * @return Formatted date string.
   */
  String 
  toString(const String& format)
  {
    return Utils::timeFormat(*this, format);
  }
};

/**
 * @class Time
 * @brief Time module providing clock conversion and frame delta tracking.
 */
class CY_UTILITY_EXPORT Time : public Module<Time> {
public:

  Time() = default;

  ~Time() = default;

  virtual void
  onStartUp() override;

  /**
   * @brief Initializes internal timer state.
   */
  void 
  init();

  /**
   * @brief Converts a system clock time point to Date.
   * @param timePoint System clock time point.
   * @return Converted date.
   */
  static Date
  scToDate(TimePoint<SystemClock> timePoint);

  /**
   * @brief Returns current local date/time.
   * @return Current local date/time.
   */
  static Date 
  now();

  /**
   * @brief Returns current UTC date/time.
   * @return Current UTC date/time.
   */
  static Date
  nowUTC();

  /**
   * @brief Updates frame timing values.
   */
  void 
  update();

  /**
   * @brief Returns frame delta in the requested units.
   * @param delta Units to return.
   * @return Frame delta value.
   */
  float
  deltaTime(DELTA_TYPE::E delta = DELTA_TYPE::E::eMILLISECOND);

private:

  /**
   * @brief Last high-resolution timer sample.
   */
  TimePoint<HighClock> m_timer;

  /**
   * @brief Last computed delta value.
   */
  float m_delta;

};
}

