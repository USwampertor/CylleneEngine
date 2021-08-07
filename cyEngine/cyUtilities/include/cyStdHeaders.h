/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file h
 * @author Marco "Swampy" Millan
 * @date 2018/10/17 2018
 * @brief the std headers for the container used in the engine, USE USING_EASTL
 * for EA standard library containers
 */
 /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once

#include <cassert>
#include <cmath>
#include <cstring>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <sstream>
#include <type_traits>


//#define USING_EASTL

#ifndef  USING_EASTL
//STD objects
# include <algorithm>
# include <string>
//STD Containers
# include <array>
# include <vector>
# include <deque>
# include <forward_list>
# include <list>
# include <stack>
# include <queue>
# include <set>
# include <map>
#else
//STD objects
# include <EASTL/algorithm.h>
# include <EASTL/allocator.h>
# include <EASTL/string.h>

//STD Containers
# include <EASTL/array.h>
# include <EASTL/vector.h>
# include <EASTL/deque.h>
# include <EASTL/list.h>
# include <EASTL/stack.h>
# include <EASTL/queue.h>
# include <EASTL/priority_queue.h>
# include <EASTL/set.h>
# include <EASTL/map.h>
#endif

namespace CYLLENE_SDK {
  using StringStream  = std::stringstream;

  template<typename T>
  using SharedPointer = std::shared_ptr<T>;

  using Mutex         = std::mutex;

  using HighClock     = std::chrono::high_resolution_clock;

  using SteadyClock   = std::chrono::steady_clock;

  using SystemClock   = std::chrono::system_clock;

  using TimeType      = std::time_t;

  using TM            = std::tm;

  template<typename T>
  using TimePoint     = std::chrono::time_point<T>;

  using Nanoseconds   = std::chrono::nanoseconds;

  using Miliseconds   = std::chrono::milliseconds;

  using Microseconds  = std::chrono::microseconds;

  using Seconds       = std::chrono::seconds;

  using Minutes       = std::chrono::minutes;

  using Hours         = std::chrono::hours;

  template<typename T>
  using NumericLimits = std::numeric_limits<T>;


}


namespace CYLLENE_SDK {
#ifndef USING_EASTL
  template<typename T, size_t A>
  using Array = std::array<T, A>;

  template<typename T, typename A = std::allocator<T>>
  using Vector = std::vector<T, A>;

  template<typename T, typename A = std::allocator<T>>
  using Deque = std::deque<T, A>;

  template<typename T, typename A = std::allocator<T>>
  using ForwardList = std::forward_list<T, A>;

  template<typename T, typename A = std::allocator<T>>
  using List = std::list<T, A>;

  template<typename T, typename A = std::deque<T>>
  using Stack = std::stack<T, A>;

  template<typename T, typename A = std::deque<T>>
  using Queue = std::queue<T, A>;

  template<typename T, typename A = std::vector<T>>
  using PriorityQueue = std::priority_queue<T, A>;

  template<typename T, typename B = std::less<T>, typename A = std::allocator<T>>
  using Set = std::set<T, B, A>;

  template<typename T, typename B = std::less<T>, typename A = std::allocator<T>>
  using MultiSet = std::multiset< T, B, A >;

  template< typename KEY,
    typename T,
    typename B = std::less<KEY>,
    typename A = std::allocator<std::pair<const KEY, T>>>
    using Map = std::map<KEY, T, B, A>;

  template< typename KEY,
    typename T,
    typename B = std::less<KEY>,
    typename A = std::allocator<std::pair<const KEY, T>>>
    using MultiMap = std::multimap<KEY, T, B, A>;

  using String = std::string;

#else
  template<typename T, size_t N = 1>
  using Array = eastl::array<T, N>;

  template<typename T, typename A = EASTLAllocatorType>
  using Vector = eastl::vector<T, A>;

  template<typename T,
    typename A = EASTLAllocatorType,
    unsigned kDequeSubarraySize = DEQUE_DEFAULT_SUBARRAY_SIZE(T)>
    using Deque = eastl::deque<T, A, kDequeSubarraySize>;

  template<typename T, typename A = EASTLAllocatorType>
  using ForwardList = eastl::list<T, A>;

  template<typename T, typename A = EASTLAllocatorType>
  using List = eastl::list<T, A>;

  template<typename T, typename A = eastl::vector<T>>
  using Stack = eastl::stack<T, A>;

  template<typename T,
    typename A = eastl::deque<T, EASTLAllocatorType, DEQUE_DEFAULT_SUBARRAY_SIZE(T)>>
    using Queue = eastl::queue<T, A>;

  template<typename T,
    typename B = eastl::vector<T>,
    typename A = eastl::less<typename B::value_type> >
    using PriorityQueue = eastl::priority_queue<T, B, A>;

  template<typename KEY,
    typename B = eastl::less<KEY>,
    typename A = EASTLAllocatorType>
    using Set = eastl::set<KEY, B, A>;

  template<typename KEY,
    typename B = eastl::less<KEY>,
    typename A = EASTLAllocatorType>
    using MultiSet = eastl::multiset<KEY, B, A>;

  template<typename KEY, typename T,
    typename B = eastl::less<KEY>,
    typename A = EASTLAllocatorType>
    using Map = eastl::map<KEY, B, A>;

  template<typename KEY, typename T,
    typename B = eastl::less<KEY>,
    typename A = EASTLAllocatorType>
    using MultiMap = eastl::multimap<KEY, B, A>;

  using String = eastl::string;


#endif

}