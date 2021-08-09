/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyMemoryAllocator.h
 * @author Marco "Swampy" Millan
 * @date 8/4/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once

#undef min
#undef max

#if CY_PLATFORM == CY_PLATFORM_LINUX
# include <malloc.h>
#endif

#include "cyPlatformTypes.h"
#include "cyPlatformDefines.h"
#include "cyStdHeaders.h"

#include <new>
#include <atomic>
#include <utility>

namespace CYLLENE_SDK {
  using std::forward;
  using std::ptrdiff_t;

  class MemoryAllocatorBase;

#if CY_PLATFORM == CY_PLATFORM_WIN32
  inline void*
    platformAlignedAlloc16(SIZE_T size) {
    return _aligned_malloc(size, 16);
  }

  inline void
    platformAlignedFree16(void* ptr) {
    _aligned_free(ptr);
  }

  inline void*
    platformAlignedAlloc(SIZE_T size, SIZE_T alignment) {
    return _aligned_malloc(size, alignment);
  }

  inline void
    platformAlignedFree(void* ptr) {
    _aligned_free(ptr);
  }
#elif CY_PLATFORM == CY_PLATFORM_LINUX || CY_PLATFORM == CY_PLATFORM_ANDROID
  inline void*
    platformAlignedAlloc16(SIZE_T size) {
    return ::memalign(16, size);
  }

  inline void
    platformAlignedFree16(void* ptr) {
    ::free(ptr);
  }

  inline void*
    platformAlignedAlloc(SIZE_T size, SIZE_T alignment) {
    return ::memalign(alignment, size);
  }

  inline void
    platformAlignedFree(void* ptr) {
    ::free(ptr);
  }
#else //16 byte alignment by default
  inline void*
    platformAlignedAlloc16(SIZE_T size) {
    return ::malloc(size);
  }

  inline void
    platformAlignedFree16(void* ptr) {
    ::free(ptr);
  }

  inline void*
    platformAlignedAlloc(SIZE_T size, SIZE_T alignment) {
    void* data = ::malloc(size + (alignment - 1) + sizeof(void*));
    if (nullptr == data) {
      return nullptr;
    }

    uint8* alignedData = (reinterpret_cast<uint8*>(data)) + sizeof(void*);
    alignedData += alignment - (reinterpret_cast<uintptr_t>(alignedData)) & (alignment - 1);

    (reinterpret_cast<void**>(alignedData))[-1] = data;
    return alignedData;
  }

  inline void
    platformAlignedFree(void* ptr) {
    ::free(((void**)ptr)[-1]);
  }
#endif

  /**
   * @class MemoryCounter
   * @brief Thread safe class used for storing total number of memory
   *        allocations and deallocations, primarily for statistic purposes.
   */
  class MemoryCounter
  {
  public:
    static CY_UTILITY_EXPORT uint64
      getNumAllocs() {
      return m_allocs;
    }

    static CY_UTILITY_EXPORT uint64
      getNumFrees() {
      return m_frees;
    }

  private:
    friend class MemoryAllocatorBase;

    /**
     * Thread local data can't be exported, so some magic to make it accessible
     * from MemoryAllocator
     */
    static CY_UTILITY_EXPORT void
      incrementAllocCount() {
      ++m_allocs;
    }

    static CY_UTILITY_EXPORT void
      incrementFreeCount() {
      ++m_frees;
    }

    static CY_THREADLOCAL uint64 m_allocs;
    static CY_THREADLOCAL uint64 m_frees;
  };

  /**
   * @class MemoryAllocatorBase
   * @brief Base class all memory allocators need to inherit.
   *        Provides allocation and free counting.
   */
  class MemoryAllocatorBase
  {
  protected:
    static void
      incrementAllocCount() {
      MemoryCounter::incrementAllocCount();
    }

    static void
      incrementFreeCount() {
      MemoryCounter::incrementFreeCount();
    }
  };

  /**
   * @class MemoryAllocator
   * @brief Provides a generic implementation. Specialize for specific
   *        categories as needed. For example you might implement a pool
   *        allocator for specific types in order to reduce allocation overhead.
   *        By default standard malloc/free are used.
   */
  template<class T>
  class MemoryAllocator : public MemoryAllocatorBase
  {
  public:
    static void*
      allocate(SIZE_T bytes) {
#if CY_PROFILING_ENABLED
      incrementAllocCount();
#endif
      return malloc(bytes);
    }

    /**
     * @brief Allocates @p bytes and aligns them to the specified boundary (in bytes).
     *        If the alignment is less or equal to 16 it is more efficient to use the
     *        allocateAligned16() alternative of this method.
     *        Alignment must be power of two.
     */
    static void*
      allocateAligned(SIZE_T bytes, SIZE_T alignment) {
#if CY_PROFILING_ENABLED
      incrementAllocCount();
#endif
      return platformAlignedAlloc(bytes, alignment);
    }

    /**
     * @brief Allocates @p bytes and aligns them to a 16 byte boundary.
     */
    static void*
      allocateAligned16(SIZE_T bytes) {
#if CY_PROFILING_ENABLED
      incrementAllocCount();
#endif
      return platformAlignedAlloc16(bytes);
    }

    static void
      free(void* ptr) {
#if CY_PROFILING_ENABLED
      incrementFreeCount();
#endif
      ::free(ptr);
    }

    /**
     * @brief Frees memory allocated with allocateAligned()
     */
    static void
      freeAligned(void* ptr) {
#if CY_PROFILING_ENABLED
      incrementFreeCount();
#endif
      platformAlignedFree(ptr);
    }

    /**
     * @brief Frees memory allocated with allocateAligned16()
     */
    static void
      freeAligned16(void* ptr) {
#if CY_PROFILING_ENABLED
      incrementFreeCount();
#endif
      platformAlignedFree16(ptr);
    }
  };

  /**
   * @brief General allocator provided by the OS. Use for persistent long term
   *        allocations, and allocations that don't happen often.
   */
  class GenAlloc
  {};

  /**
   * @brief Allocates the specified number of bytes.
   */
  template<class Alloc>
  inline void*
    cy_alloc(SIZE_T count) {
    return MemoryAllocator<Alloc>::allocate(count);
  }

  /**
   * @brief Allocates enough bytes to hold the specified type, but doesn't construct it.
   */
  template<class T, class Alloc>
  inline T*
    cy_alloc() {
    return reinterpret_cast<T*>(MemoryAllocator<Alloc>::allocate(sizeof(T)));
  }

  /**
   * @brief Creates and constructs an array of "count" elements.
   */
  template<class T, class Alloc>
  inline T*
    cy_newN(SIZE_T count) {
    auto ptr = reinterpret_cast<T*>(MemoryAllocator<Alloc>::allocate(sizeof(T) * count));

    for (SIZE_T i = 0; i < count; ++i) {
      new (&ptr[i]) T;
    }

    return ptr;
  }

  /**
   * @brief Create a new object with the specified allocator and the specified parameters.
   */
  template<class T, class Alloc, class... Args>
  T*
    cy_new(Args&& ...args) {
    return new (cy_alloc<T, Alloc>()) T(forward<Args>(args)...);
  }

  /**
   * @brief Frees all the bytes allocated at the specified location.
   */
  template<class Alloc>
  inline void
    cy_free(void* ptr) {
    MemoryAllocator<Alloc>::free(ptr);
  }

  /**
   * @brief Destructs and frees the specified object.
   */
  template<class T, class Alloc = GenAlloc>
  inline void
    cy_delete(T* ptr) {
    (ptr)->~T();
    MemoryAllocator<Alloc>::free(ptr);
  }

  /**
   * @brief Destructs and frees the specified array of objects.
   */
  template<class T, class Alloc = GenAlloc>
  inline void
    cy_deleteN(T* ptr, SIZE_T count) {
    for (SIZE_T i = 0; i < count; ++i) {
      ptr[i].~T();
    }
    MemoryAllocator<Alloc>::free(ptr);
  }

  /***************************************************************************/
  /**
   * Default versions of all alloc/free/new/delete methods which call GenAlloc
   */
   /***************************************************************************/

   /**
    * @brief Allocates the specified number of bytes.
    */
  inline void*
    cy_alloc(SIZE_T count) {
    return MemoryAllocator<GenAlloc>::allocate(count);
  }

  /**
   * @brief Allocates enough bytes to hold the specified type, but doesn't construct it.
   */
  template<class T>
  inline T*
    cy_alloc() {
    return reinterpret_cast<T*>(MemoryAllocator<GenAlloc>::allocate(sizeof(T)));
  }

  /**
   * @brief Allocates the specified number of bytes aligned to the provided boundary.
   *        Boundary is in bytes and must be a power of two.
   */
  inline void*
    cy_alloc_aligned(SIZE_T count, SIZE_T align) {
    return MemoryAllocator<GenAlloc>::allocateAligned(count, align);
  }

  /**
   * @brief Allocates the specified number of bytes aligned to a 16 bytes boundary.
   */
  inline void*
    cy_alloc_aligned16(SIZE_T count) {
    return MemoryAllocator<GenAlloc>::allocateAligned16(count);
  }

  /**
   * @brief Creates and constructs an array of "count" elements.
   */
  template<class T>
  inline T*
    cy_allocN(SIZE_T count) {
    return reinterpret_cast<T*>(MemoryAllocator<GenAlloc>::allocate(sizeof(T) * count));
  }

  /**
  * @brief Creates and constructs an array of "count" elements.
  */
  template<class T>
  inline T*
    cy_newN(SIZE_T count) {
    T* ptr = reinterpret_cast<T*>(MemoryAllocator<GenAlloc>::allocate(sizeof(T) * count));
    for (SIZE_T i = 0; i < count; ++i) {
      new (&ptr[i]) T;
    }

    return ptr;
  }

  /**
   * @brief Create a new object with the specified allocator and the specified parameters.
   */
  template<class T, class... Args>
  T*
    cy_new(Args&& ...args) {
    return new (cy_alloc<T, GenAlloc>()) T(forward<Args>(args)...);
  }

  /**
   * @brief Frees all the bytes allocated at the specified location.
   */
  inline void
    cy_free(void* ptr) {
    MemoryAllocator<GenAlloc>::free(ptr);
  }

  /**
   * @brief Frees memory previously allocated with ge_alloc_aligned().
   */
  inline void
    cy_free_aligned(void* ptr) {
    MemoryAllocator<GenAlloc>::freeAligned(ptr);
  }

  /**
   * @brief Frees memory previously allocated with ge_alloc_aligned16().
   */
  inline void
    cy_free_aligned16(void* ptr) {
    MemoryAllocator<GenAlloc>::freeAligned16(ptr);
  }

  /***************************************************************************/
  /**                     MACRO VERSIONS
   * You will almost always want to use the template versions but in some cases
   * (private constructor / destructor) it is not possible. In which case you
   * may use these instead.
   */
   /***************************************************************************/
#define CY_PVT_NEW(T, ...)                                                    \
      new (cy_alloc<T, GenAlloc>()) T(__VA_ARGS__)

#define CY_PVT_DELETE(T, ptr)                                                 \
      (ptr)->~T();                                                            \
      MemoryAllocator<GenAlloc>::free(ptr);

#define CY_PVT_DELETE_A(T, ptr, Alloc)                                        \
      (ptr)->~T();                                                            \
      MemoryAllocator<Alloc>::free(ptr);

  /**
   * @brief Allocator for the standard library that internally uses the
   *        Genesis Engine memory allocator.
   */
  template <class T, class Alloc = GenAlloc>
  class StdAlloc
  {
  public:
    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = SIZE_T;
    using difference_type = ptrdiff_t;

    constexpr StdAlloc() = default;
    constexpr StdAlloc(StdAlloc&&) = default;
    constexpr StdAlloc(const StdAlloc&) = default;

    template<class U, class Alloc2>
    constexpr StdAlloc(const StdAlloc<U, Alloc2>&) _NOEXCEPT {}

    template<class U, class Alloc2>
    constexpr bool
      operator==(const StdAlloc<U, Alloc2>&) const _NOEXCEPT {
      return true;
    }

    template<class U, class Alloc2>
    constexpr bool
      operator!=(const StdAlloc<U, Alloc2>&) const _NOEXCEPT {
      return false;
    }

    template<class U>
    class rebind
    {
    public:
      using other = StdAlloc<U, Alloc>;
    };

    /**
     * @brief Allocate but don't initialize number elements of type T.
     */
    static T*
      allocate(const SIZE_T num) {
      if (0 == num) {
        return nullptr;
      }

      if (num > NumericLimits<SIZE_T>::max() / sizeof(T)) {
        throw nullptr;
      }

      void* const pv = cy_alloc<Alloc>(num * sizeof(T));
      if (!pv) {
        return nullptr;
      }

      return static_cast<T*>(pv);
    }

    /**
     * @brief Deallocate storage p of deleted elements.
     */
    static void
      deallocate(pointer p, SIZE_T) {
      cy_free<Alloc>(p);
    }

    static constexpr SIZE_T
      max_size() {
      return NumericLimits<SIZE_T>::max() / sizeof(T);
    }

    static constexpr void
      destroy(pointer p) {
      p->~T();
    }

    /**
     * @brief This version of construct() (with a varying number of parameters)
     *        seems necessary in order to use some STL data structures from
     *        libstdc++-4.8, but compilation fails on OSX, hence the #if.
     */
    template<class... Args>
    static void
      construct(pointer p, Args&& ...args) {
      new(p) T(forward<Args>(args)...);
    }
  };
}
