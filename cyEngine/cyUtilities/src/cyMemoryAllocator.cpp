#include "cyMemoryAllocator.h"

namespace CYLLENE_SDK {
  CY_THREADLOCAL uint64 MemoryCounter::m_allocs = 0;
  CY_THREADLOCAL uint64 MemoryCounter::m_frees  = 0;
}