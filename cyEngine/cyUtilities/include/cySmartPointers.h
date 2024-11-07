#pragma once

#include "cyStdHeaders.h"
#include "cyMemoryAllocator.h"
#include "cyModule.h"

namespace CYLLENE_SDK
{

class ManagedPtr {
public:
  virtual ~ManagedPtr() = default;
};

template <typename T>
class SmartPtr;

template <typename T>
class WeePtr {
public:
  // Constructor - gets the pointer from a UniquePointer
  explicit WeePtr(SmartPtr<T>& uniquePtr) : m_ptr(uniquePtr.get()) {}

  // Accessors to use the weak pointer safely
  T* get() const { return m_ptr; }

  // Checks if the object still exists
  bool expired() const { return m_ptr == nullptr; }

  // Dereference operator for easy access
  T& operator*() const { return *m_ptr; }
  T* operator->() const { return m_ptr; }

private:
  T* m_ptr; // Raw pointer to the managed resource, without ownership

};

template <typename T>
class SmartPtr : public ManagedPtr {
public:

  explicit SmartPtr(T* p = nullptr) : m_ptr(p) {}
  
  ~SmartPtr() { delete m_ptr; }

  // Disallow copy
  SmartPtr(const SmartPtr&) = delete;
  
  SmartPtr& operator=(const SmartPtr&) = delete;

  // Move semantics
  SmartPtr(SmartPtr&& other) noexcept : m_ptr(other.m_ptr) {
    other.m_ptr = nullptr;
  }

  SmartPtr& operator=(SmartPtr&& other) noexcept {
    if (this != &other) {
      delete m_ptr;
      m_ptr = other.m_ptr;
      other.m_ptr = nullptr;
    }
    return *this;
  }

  T& operator*() const { return *m_ptr; }
  T* operator->() const { return m_ptr; }
  T* get() const { return m_ptr; }

  void reset(T* newPtr = nullptr) {
    delete m_ptr;
    m_ptr = newPtr;
  }

  // Method to create a WeakPointer
  WeePtr<T> ptr() {
    return WeePtr<T>(*this);
  }

  template <typename U>
  friend SmartPtr<U> reinterpret_smart_cast(SmartPtr<T>&& uptr);

private:
  T* m_ptr;

};

// reinterpret_pointer_cast function
template <typename U, typename T>
SmartPtr<U> reinterpret_smart_cast(SmartPtr<T>&& uptr) {
  U* castedPtr = reinterpret_cast<U*>(uptr.get());
  uptr.ptr = nullptr;  // Release the ownership from the original pointer
  return SmartPtr<U>(castedPtr); // Return the new UniquePointer with casted type
}


class SmartPointers : public Module<SmartPointers> {
private:
  UnorderedSet<ManagedPtr*> pointers;

public:
  ~SmartPointers() {
    // Cleanup all pointers
    cleanup();
  }

  // Store a UniquePointer and return the raw pointer
  template <typename T>
  T* store(SmartPtr<T>&& uniquePtr) {
    SmartPtr<T>* newPtr = new SmartPtr<T>(std::move(uniquePtr));
    pointers.insert(newPtr);
    return newPtr->get();
  }

  // Garbage collection to clean up dangling pointers
  void cleanup() {
    for (auto ptr : pointers) {
      delete ptr;
    }
    pointers.clear();
  }

  // Optional: Remove a specific pointer if needed
  template <typename T>
  void remove(T* rawPtr) {
    for (auto it = pointers.begin(); it != pointers.end(); ++it) {
      if (*it == rawPtr) {
        delete* it;
        pointers.erase(it);
        break;
      }
    }
  }
};


// FUNCTIONS

template <typename T, typename... Args>
UniquePointer<T> MakeUniqueObject(Args ... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
SharedPointer<T> MakeSharedObject(Args ... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
T* MakeObject(Args&&... args) {
  SmartPtr<T> uniquePtr(new T(std::forward<Args>(args)...));
  return SmartPointers::isStarted() ? SmartPointers::instance().store(std::move(uniquePtr)) : nullptr;
}

#define CY_MAKEUNIQUE(T, ...) std::make_unique<T>(__VA_ARGS__)

#define CY_MAKESHARED(T, ...) std::make_shared<T>(__VA_ARGS__)

#define CY_MAKEOBJECT(T, ...) MakeObject<T>(_VA_ARGS__)

#define REINTERPRETPOINTER(T, ...) std::reinterpret_pointer_cast<T>(__VA_ARGS__)

#define STATICPOINTER(T, ...) std::static_pointer_cast<T>(__VA_ARGS__)

#define STATICCAST(T, ...) std::static_cast<T>(__VA_ARGS__)

#define REINTERPRETSMART(T, ...) reinterpret_smart_cast<T>(__VA_ARGS__)

}
