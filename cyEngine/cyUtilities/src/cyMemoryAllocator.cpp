/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyMemoryAllocator.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/04/05 2019
 * @brief Memory allocator definition of variables
 *
 */
 /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK {
  CY_THREADLOCAL uint64 MemoryCounter::m_allocs = 0;
  CY_THREADLOCAL uint64 MemoryCounter::m_frees = 0;
}