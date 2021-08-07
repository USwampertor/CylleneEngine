/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyDate.h
 * @author Marco "Swampy" Millan
 * @date 8/6/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyUtilities.h"
#include "cyModule.h"

namespace CYLLENE_SDK {

  namespace DELTA_TYPE
  {
    BETTER_ENUM(E, uint32, 
      MICROSECOND = 0,
      MILLISECOND,
      SECOND)
  }

  struct CY_UTILITY_EXPORT Date : public TM
  {
  public:
    
    Date() = default;

    ~Date() = default;

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

    Date operator=(const TM& father) {
      return Date(father);
    }

    String 
    toString(const String& format)
    {
      char buffer[128];
      std::strftime(buffer, sizeof(buffer), format.c_str(), this);
      return String(buffer);
    }
  };


  class CY_UTILITY_EXPORT Time : public Module<Time> {
  public:

    Time() = default;

    ~Time() = default;

    void 
    init();

    static Date 
    now();

    static Date
    nowUTC();

    void 
    update();

    float
    deltaTime();

    float
    deltaTime(DELTA_TYPE::E& delta);

  private:

    TimePoint<HighClock> m_timer;


    float m_delta;


  };
}

