#include "cyCountAllocator.h"
#include "cyCountAllocatorPointer.h"
#include "cyUtilities.h"

#include <algorithm>

namespace CYLLENE_SDK {

CountAllocator::CountAllocator()
  : m_initialized(false),
    m_memory(nullptr),
    m_memorySize(0) {
}

CountAllocator::~CountAllocator() {
}

bool
CountAllocator::Allocate(SizeT size) {
  if (m_initialized) {
    throw CountAllocatorExceptions::kMemoryAlreadyInitialized;
    return false;
  }

  if (m_memory != nullptr) {
    throw CountAllocatorExceptions::kMemoryPointingToSomewhere;
    return false;
  }

  m_initialized = true;
  m_memory = malloc(size);

  m_memoryBlocks.clear();
  m_memoryBlocks.insert(CAMemoryBlock(0, size));

  return true;
}

bool
CountAllocator::Deallocate() {
  if (!m_initialized) {
    throw CountAllocatorExceptions::kMemoryNotInitialized;
    return false;
  }

  if (m_memory == nullptr) {
    throw CountAllocatorExceptions::kNullMemory;
    return false;
  }
  
  m_initialized = false;
  free(m_memory);

  return true;
}

void
CountAllocator::Defragment() {
  // Not implemented yet.
  
  /**
  * TODO: Search for a cool way to organize memory, maybe the
  * hanoi tower algorith is a good start?
  */
}

void
CountAllocator::ReleasePointer(CAMemoryBlock pointer) {
  CAMemoryBlock newBlock = pointer;

  auto is_prev_block = [&newBlock](CAMemoryBlock block) {
    return (block.offset + block.size) == newBlock.offset;
  };

  auto is_next_block = [&newBlock](CAMemoryBlock block) {
    return (newBlock.offset + newBlock.size) == block.offset;
  };

  auto prevBlock = std::find_if(m_memoryBlocks.begin(),
                                m_memoryBlocks.end(),
                                is_prev_block);

  auto nextBlock = std::find_if(m_memoryBlocks.begin(),
                                m_memoryBlocks.end(),
                                is_next_block);

  if (prevBlock != m_memoryBlocks.end()) {
    newBlock.offset -= prevBlock->size;
    newBlock.size   += prevBlock->size;
    m_memoryBlocks.erase(prevBlock);
  }

  if (nextBlock != m_memoryBlocks.end()) {
    newBlock.size += nextBlock->size;
    m_memoryBlocks.erase(nextBlock);
  }

  m_memoryBlocks.insert(newBlock);
}

}