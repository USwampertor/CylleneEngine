/**
 * @file cySmartPointers.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for SmartPointers.
 */

#pragma once

#include "cyStdHeaders.h"
#include "cyMemoryAllocator.h"
#include "cyModule.h"
#include "cyPlatformDefines.h"
#include "cyAssert.h"

namespace CYLLENE_SDK
{

template<typename T>
using SPtr = std::shared_ptr<T>;

template<typename T>
using UPtr = std::unique_ptr<T>;

template<typename T>
using WPtr = std::weak_ptr<T>;

/**
 * @class ManagedPtr
 * @brief Base managed pointer state containing shared reference counter.
 */
class ManagedPtr {
public:
  /**
   * @brief Virtual destructor.
   */
  virtual ~ManagedPtr() = default;
  
  /**
   * @brief Returns reference counter storage.
   * @return Reference counter reference.
   */
  NODISCARD FORCEINLINE uint32& 
  getCounter() { return m_counter; }
  
  friend class SmartPointers;

protected:
  /**
   * @brief Reference counter for smart pointer wrappers.
   */
  uint32 m_counter;
};

template <typename T>
class SmartPtr;

/**
 * @class SmallPtr
 * @brief Non-owning lightweight handle bound to a SmartPtr instance.
 * @tparam T Managed object type.
 */
template <typename T>
class SmallPtr {
public:

  /**
   * @brief Constructs a non-owning handle from a SmartPtr.
   * @param uniquePtr Source smart pointer.
   */
  explicit SmallPtr(SmartPtr<T>& uniquePtr)
    : m_ref(&uniquePtr) {
    ++uniquePtr.m_counter;
  }

  /**
   * @brief Destroys the non-owning handle.
   */
  ~SmallPtr() { --m_ref->m_counter; }

  /**
   * @brief Rebinds this handle to another SmartPtr.
   * @param other New smart pointer reference.
   * @return Reference to this handle.
   */
  SmallPtr& operator=(SmartPtr<T>& other)
  {
    --m_ref->m_counter;
    m_ref = &other;
    ++m_ref->m_counter;
    return *this;
  }

  /**
   * @brief Checks whether referenced object is expired.
   * @return True if object pointer is null.
   */
  bool 
  expired() const { return m_ref->get() == nullptr; }

  /**
   * @brief Dereferences the referenced object.
   * @return Reference to managed object.
   */
  T& operator*() const { return *m_ref->get(); }

  /**
   * @brief Accesses the referenced object pointer.
   * @return Pointer to managed object.
   */
  T* operator->() const { return m_ref->get(); }

private:
  
  /**
   * @brief Referenced smart pointer owner.
   */
  SmartPtr<T>* m_ref;
};

/**
 * @class SmartPtr
 * @brief Owning pointer wrapper with simple move semantics and ref count support.
 * @tparam T Managed object type.
 */
template <typename T>
class SmartPtr : public ManagedPtr {
public:

  /**
   * @brief Constructs a smart pointer from a raw pointer.
   * @param p Raw pointer to manage.
   */
  explicit SmartPtr(T* p = nullptr) 
    : m_ptr(p) {
      m_counter = 1;
  }
  
  /**
   * @brief Destroys managed object.
   */
  ~SmartPtr() { 
    m_counter = 0; 
    delete m_ptr; 
  }

  /**
   * @brief Copy constructor is disabled.
   */
  SmartPtr(const SmartPtr&) = delete;
  
  /**
   * @brief Copy assignment is disabled.
   */
  SmartPtr& operator=(const SmartPtr&) = delete;

  /**
   * @brief Move constructor.
   * @param other Source smart pointer.
   */
  SmartPtr(SmartPtr&& other) noexcept 
    : m_ptr(other.m_ptr) {
    m_counter = other.getCounter();
    other.m_ptr = nullptr;
  }

  /**
   * @brief Move assignment.
   * @param other Source smart pointer.
   * @return Reference to this smart pointer.
   */
  SmartPtr& operator=(SmartPtr&& other) noexcept {
    if (this != &other) {
      delete m_ptr;
      m_ptr = other.m_ptr;
      other.m_ptr = nullptr;
    }
    return *this;
  }

  /**
   * @brief Dereferences the managed object.
   * @return Reference to managed object.
   */
  T& operator*() const { return *m_ptr; }

  /**
   * @brief Accesses the managed object pointer.
   * @return Raw managed pointer.
   */
  T* operator->() const { return m_ptr; }

  /**
   * @brief Resets managed pointer.
   * @param newPtr Replacement raw pointer.
   */
  void 
  reset(T* newPtr = nullptr) {
    delete m_ptr;
    m_ptr = newPtr;
  }

  // Method to create a WeakPointer
  /**
   * @brief Creates a SmallPtr handle.
   * @return SmallPtr linked to this SmartPtr.
   */
  SmallPtr<T> 
  ptr() {
    return SmallPtr<T>(*this);
  }

  template <typename U>
  friend SmartPtr<U> reinterpret_smart_cast(SmartPtr<T>&& uptr);

  friend class SmartPointers;
  friend class SmallPtr<T>;

private:
  
  /**
   * @brief Gets raw managed pointer.
   * @return Managed raw pointer.
   */
  T* get() const { return m_ptr; }

  /**
   * @brief Raw managed pointer.
   */
  T* m_ptr;
};

/**
 * @brief Reinterprets one SmartPtr type as another.
 * @tparam U Destination pointer type.
 * @tparam T Source pointer type.
 * @param uptr Source smart pointer.
 * @return Reinterpreted smart pointer.
 */
template <typename U, typename T>
SmartPtr<U> reinterpret_smart_cast(SmartPtr<T>&& uptr) {
  // U* castedPtr = reinterpret_cast<U*>(uptr.get());
  // uptr.ptr = nullptr;  // Release the ownership from the original pointer
  // return SmartPtr<U>(castedPtr); // Return the new UniquePointer with casted type
  // TODO: Check iof this is the correct thing to do
  return *reinterpret_cast<SmartPtr<U>*>(&uptr);
}

/**
 * @class SmartPointers
 * @brief Manager module tracking allocated SmartPtr wrappers.
 */
class SmartPointers : public Module<SmartPointers> {

public:
  /**
   * @brief Destructor that performs cleanup.
   */
  ~SmartPointers() {
    cleanup();
  }

  /**
   * @brief Stores an external SmartPtr wrapper in the manager.
   * @tparam T Managed object type.
   * @param uniquePtr Smart pointer to store.
   * @return Raw pointer to managed object.
   */
  template <typename T>
  T* 
  store(SmartPtr<T>&& uniquePtr) {
    SmartPtr<T>* newPtr = cy_new<SmartPtr<T>>(std::move(uniquePtr));
    pointers.insert(newPtr);
    return newPtr->get();
  }

  /**
   * @brief Removes tracked wrappers with low reference count.
   */
  void 
  cleanup() {
    int32 size = 0;
    for (auto ptr : pointers) {
      if (ptr->m_counter <= 1) {
        cy_delete(ptr);
        ++size;
      }
    }
    if (size == pointers.size()) {
      pointers.clear();
    }
  }

  /**
   * @brief Creates and tracks a managed object.
   * @tparam T Managed object type.
   * @tparam Args Constructor argument types.
   * @param args Constructor arguments.
   * @return Created SmartPtr wrapper.
   */
  template <typename T, typename... Args>
  SmartPtr<T> 
  create(Args ... args) {
    SmartPtr<T>* newPtr = cy_new<SmartPtr<T>>(std::forward<Args>(args)...);
    pointers.insert(newPtr);
    return *newPtr;
  }
  
    
  /**
   * @brief Removes a tracked wrapper by raw pointer.
   * @tparam T Pointer type.
   * @param rawPtr Pointer to tracked wrapper.
   */
  template <typename T>
  void 
  remove(T* rawPtr) {
    for (auto it = pointers.begin(); it != pointers.end(); ++it) {
      if (*it == rawPtr) {
        cy_delete(*it);
        pointers.erase(it);
        break;
      }
    }
  }

private:

  /**
   * @brief Tracked managed-pointer wrappers.
   */
  UnorderedSet<ManagedPtr*> pointers;

};

/**
 * @brief Creates a `std::unique_ptr`.
 * @tparam T Managed type.
 * @tparam Args Constructor argument types.
 * @param args Constructor arguments.
 * @return Created unique pointer.
 */
template <typename T, typename... Args>
UPtr<T> makeUniquePtr(Args ... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

/**
 * @brief Creates a `std::shared_ptr`.
 * @tparam T Managed type.
 * @tparam Args Constructor argument types.
 * @param args Constructor arguments.
 * @return Created shared pointer.
 */
template <typename T, typename... Args>
SPtr<T> makeSharedPtr(Args ... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

/**
 * @brief Creates an engine SmartPtr via SmartPointers module.
 * @tparam T Managed type.
 * @tparam Args Constructor argument types.
 * @param args Constructor arguments.
 * @return Created smart pointer.
 */
template <typename T, typename... Args>
SmartPtr<T> makeSmartPtr(Args ... args) {
  CY_ASSERT(!SmartPointers::isStarted() && "Smart Pointer Manager was not started");
  return SmartPointers::instance().create<T>(std::forward<Args>(args)...);
}

/**
 * @brief Creates an object through SmartPointers and returns raw pointer.
 * @tparam T Managed type.
 * @tparam Args Constructor argument types.
 * @param args Constructor arguments.
 * @return Raw pointer to created object.
 */
template <typename T, typename... Args>
T* makePtr(Args&&... args) {
  CY_ASSERT(!SmartPointers::isStarted() && "Smart Pointer Manager was not started");
  return SmartPointers::instance().create<T>(std::forward<Args>(args)...).get();
}

#define CY_MAKEUNIQUE(T, ...) makeUniquePtr<T>(__VA_ARGS__)

#define CY_MAKESHARED(T, ...) makeSharedPtr<T>(__VA_ARGS__)

#define CY_MAKEOBJECT(T, ...) makePtr<T>(_VA_ARGS__)

#define CY_MAKESMART(T, ...) makeSmartPtr<T>(__VA_ARGS__)

#define REINTERPRETPOINTER(T, ...) std::reinterpret_pointer_cast<T>(__VA_ARGS__)

#define STATICPOINTER(T, ...) std::static_pointer_cast<T>(__VA_ARGS__)

#define STATICCAST(T, ...) std::static_cast<T>(__VA_ARGS__)

#define REINTERPRETSMART(T, ...) reinterpret_smart_cast<T>(__VA_ARGS__)

}

