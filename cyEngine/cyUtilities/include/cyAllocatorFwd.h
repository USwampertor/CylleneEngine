/**
 * @file cyAllocatorFwd.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Forward declaration to avoid looped dependencies
 */
#pragma once
namespace CYLLENE_SDK {
  class GenAlloc;
  template<typename T, typename Alloc = GenAlloc> class StdAlloc;
}