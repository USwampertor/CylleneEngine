#pragma once
#include <chrono>

namespace CYLLENE_SDK
{
using HighClock     = std::chrono::high_resolution_clock;

using SteadyClock   = std::chrono::steady_clock;

using SystemClock   = std::chrono::system_clock;

using TimeType      = std::time_t;

using TM            = std::tm;

using Nanoseconds   = std::chrono::nanoseconds;

using Miliseconds   = std::chrono::milliseconds;

using Microseconds  = std::chrono::microseconds;

using Seconds       = std::chrono::seconds;

using Minutes       = std::chrono::minutes;

using Hours         = std::chrono::hours;

template<typename T>
using TimePoint     = std::chrono::time_point<T>;
}
