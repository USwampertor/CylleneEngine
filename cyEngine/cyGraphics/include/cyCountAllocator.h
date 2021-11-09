#pragma once

#include <queue>

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK {

template<typename>
class CAPointer;

/**
* Exceptions thrown by Count Allocator class.
*/
enum class CountAllocatorExceptions {
  kMemoryAlreadyInitialized,
  kMemoryNotInitialized,
  kNullMemory,
  kMemoryPointingToSomewhere,
  kNotEnoughMemory,
};

/**
* 
*/
struct CAMemoryBlock {
  CAMemoryBlock(SizeT allocatorOffset,
                SizeT memorySize)
    : offset(allocatorOffset), size(memorySize) {
  };

  friend bool
  operator<(const CAMemoryBlock& lhs, const CAMemoryBlock& rhs) {
    return lhs.size < rhs.size;
  }

  SizeT offset;
  SizeT size;
};

/**
* This is a smart class that allocates memory, manages the pointers to
* that memory and can be defragmentized.
* 
* Warning:
*   This memory allocator can consume a lot of memory, use it
*   for big memory block sizes, and as a replacement for the stack allocator.
*/
class CountAllocator
{
 public:
  /**
  * Default constructor.
  */
  CountAllocator();

  /**
  * Default destructor.
  */
  ~CountAllocator();

  /**
  * Function used to allocate memory.
  * 
  * @param size
  *   The number of bytes to be allocated.
  * @return
  *   True if memory was allocated correctly.
  *   False if memory couldn't be allocated (throws and exception).
  */
  bool
  Allocate(SizeT size);

  /**
  * Frees Count Allocator memory.
  * 
  * @return
  *   True if memory was deallocated.
  *   False if there was a problem (throws an exception).
  */
  bool
  Deallocate();
  
  /**
  * Creates a pointer to the count memory and updates the allocator available space.
  * 
  * @param elements
  *   Number of elements to be allocated.
  * 
  * @return
  *   Pointer to allocated memory.
  */
  template<typename T>
  CAPointer<T>
  GetPointer(SizeT elements = 1);

  /**
  * Removes the bubbles between the memory blocks.
  */
  void
  Defragment();

 private:
  /**
  * Adds the memory block of the pointer to the available list.
  * Note: Also sews the memory.
  * 
  * @param pointer
  *   The pointer to release.
  */
  void
  ReleasePointer(CAMemoryBlock pointer);

  /**
  * Boolean used to determine if the memory was initialized.
  */
  bool m_initialized;
  
  /**
  * Pointer to allocated memory.
  */
  void* m_memory;

  /**
  * Size of the allocated memory.
  */
  SizeT m_memorySize;

  /**
  * List of avaible memory blocks.
  */
  std::set<CAMemoryBlock> m_memoryBlocks;

  template<typename>
  friend class CAPointer;
};

template<typename T>
CAPointer<T>
inline CountAllocator::GetPointer(SizeT elements) {
  const SizeT typeSize = sizeof(T);
  const SizeT requiredSize = typeSize * elements;

  for (auto it : m_memoryBlocks) {
    const CAMemoryBlock memoryBlock = it;

    if (memoryBlock.size >= requiredSize) {
      m_memoryBlocks.erase(it);

      if (memoryBlock.size > requiredSize) {
        m_memoryBlocks.insert(CAMemoryBlock(memoryBlock.offset + requiredSize,
                                            memoryBlock.size - requiredSize));
      }

      return CAPointer<T>(this,
                          CAMemoryBlock(memoryBlock.offset, requiredSize));
    }
  }

  return CAPointer<T>(this, CAMemoryBlock(0, 0));
}

}