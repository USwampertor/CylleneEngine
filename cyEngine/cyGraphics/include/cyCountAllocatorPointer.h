#pragma once

namespace CYLLENE_SDK {

class CountAllocator;
struct CAMemoryBlock;

template<typename CAPointerT>
class CAPointer {
 public:
  /**
  * Default constructor.
  */
  CAPointer();

  /**
  * 
  * @ param owner.
  *   
  * @param memoryBlock
  */
  CAPointer(CountAllocator* owner, CAMemoryBlock memoryBlock);

  /**
  * The destructor calls the release pointer method to its allocator owner.
  */
  ~CAPointer();

  CAPointerT* ptr;

 private:
  /**
  * Allocator that created this pointer.
  */
  CountAllocator* m_allocator;

  /**
  * The size of the pointed memory, and memory offset in the m_allocator memory.
  */
  CAMemoryBlock m_memoryBlock;
};

template<typename CAPointerT>
inline CAPointer<CAPointerT>::CAPointer()
  : ptr(nullptr),
    m_allocator(nullptr),
    m_memoryBlock(CAMemoryBlock(0, 0)) {
};

template<typename CAPointerT>
inline CAPointer<CAPointerT>::CAPointer(CountAllocator* owner,
                                        CAMemoryBlock memoryBlock)
  : m_allocator(owner),
    m_memoryBlock(memoryBlock) {
  uint8* start = reinterpret_cast<uint8*>(owner->m_memory);
  ptr = reinterpret_cast<CAPointerT*>(start + memoryBlock.offset);
}

template<typename CAPointerT>
inline CAPointer<CAPointerT>::~CAPointer() {
  m_allocator->ReleasePointer(m_memoryBlock);

  m_memoryBlock.offset = 0;
  m_memoryBlock.size = 0;
  ptr = nullptr;
};

}