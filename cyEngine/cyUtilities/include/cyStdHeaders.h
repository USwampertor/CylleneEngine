/**
 * @file cyStdHeaders.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for StdHeaders.
 */

#pragma once

#include <bitset>
// #include <cassert>
// #include <cmath>
// #include <cstring>
// #include <ctime>
// #include <chrono>
// #include <future>
// #include <limits>
// #include <memory>
// #include <mutex>
// #include <thread>
#include <type_traits>

//#define USING_EASTL

//STD objects
#include <algorithm>
//STD Containers
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "cyAllocatorFwd.h"

// TODO: See where this should go
namespace CYLLENE_SDK {

template<std::size_t N>
using Bitset = std::bitset<N>;

template<typename T>
using Hash   = std::hash<T>;

}

namespace CYLLENE_SDK {

template<typename T, size_t A>
using Array = std::array<T, A>;

template<typename T, typename A = StdAlloc<T>>
using Vector = std::vector<T, A>;

template<typename T, typename A = StdAlloc<T>>
using Deque = std::deque<T, A>;

template<typename T, typename A = StdAlloc<T>>
using ForwardList = std::forward_list<T, A>;

template<typename T, typename A = StdAlloc<T>>
using List = std::list<T, A>;

template<typename T, typename A = std::deque<T>>
using Stack = std::stack<T, A>;

template<typename T, typename A = std::deque<T>>
using Queue = std::queue<T, A>;

template<typename T, typename A = std::vector<T>>
using PriorityQueue = std::priority_queue<T, A>;

template<typename T, typename B = std::less<T>, typename A = StdAlloc<T>>
using Set = std::set<T, B, A>;

template<typename T, typename B = std::less<T>, typename A = StdAlloc<T>>
using MultiSet = std::multiset< T, B, A >;

template<typename KEY,
         typename T,
         typename B = std::less<KEY>,
         typename A = StdAlloc<std::pair<const KEY, T>>>
using Map = std::map<KEY, T, B, A>;

template<typename KEY,
         typename T,
         typename B = std::less<KEY>,
         typename A = StdAlloc<std::pair<const KEY, T>>>
using MultiMap = std::multimap<KEY, T, B, A>;

template<typename T, typename A>
using Pair = std::pair<T, A>;

template<typename T, typename A>
using Tuple = std::tuple<T, A>;

template<class Key,
         class H = Hash<Key>,
         class KeyEqual = std::equal_to<Key>,
         class Allocator = StdAlloc<Key>>
using UnorderedSet = std::unordered_set<Key, H, KeyEqual, Allocator>;

template<typename Key, 
         typename Value, 
         typename H = Hash<Key>,
         typename KeyEqual = std::equal_to<Key>,
         typename Allocator = StdAlloc<std::pair<const Key, Value>>>
using UnorderedMap = std::unordered_map<Key, Value, H, KeyEqual, Allocator>;

}
