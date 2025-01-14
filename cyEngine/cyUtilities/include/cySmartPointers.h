#pragma once

#include "cyStdHeaders.h"
#include "cyMemoryAllocator.h"
#include "cyModule.h"
#include "cyPlatformDefines.h"

namespace CYLLENE_SDK
{

class ManagedPtr {
public:
  virtual ~ManagedPtr() = default;
  
  uint32& 
  getCounter();
  
  friend class SmartPointers;

protected:
  
  uint32 m_counter;
};

template <typename T>
class SmartPtr;

template <typename T>
class SmallPtr {
public:

  explicit SmallPtr(SmartPtr<T>& uniquePtr)
    : m_ref(uniquePtr) {
    ++uniquePtr->m_counter;
  }

  ~SmallPtr() { --m_ref.m_counter; }

  SmallPtr& operator=(SmartPtr<T>& other)
  {
    --m_ref.m_counter;
    m_ref = other;
    ++m_ref.m_counter;
  }

  // Accessors to use the weak pointer safely
  // T* get() const { return m_ptr; }

  // Checks if the object still exists
  bool 
  expired() const { return m_ref.get() == nullptr; }

  // Dereference operator for easy access
  T& operator*() const { return *m_ref.get(); }

  T* operator->() const { return m_ref.get(); }

private:

  // T* m_ptr; // Raw pointer to the managed resource, without ownership
  
  SmartPtr<T> m_ref;
};

template <typename T>
class SmartPtr : public ManagedPtr {
public:

  explicit SmartPtr(T* p = nullptr) 
    : m_ptr(p) {
      m_counter = 1;
  }
  
  ~SmartPtr() { 
    m_counter = 0; 
    delete m_ptr; 
  }

  // Disallow copy
  SmartPtr(const SmartPtr&) = delete;
  
  // Disallow copy
  SmartPtr& operator=(const SmartPtr&) = delete;

  // Move semantics
  SmartPtr(SmartPtr&& other) noexcept 
    : m_ptr(other.m_ptr) {
    m_counter = other.getCounter();
    other.m_ptr = nullptr;
  }

  SmartPtr& operator=(SmartPtr&& other) noexcept {
    if (this != &other) {
      delete m_ptr;
      m_ptr = other.getCounter();
      other.m_ptr = nullptr;
    }
    return *this;
  }

  T& operator*() const { return *m_ptr; }

  T* operator->() const { return m_ptr; }
  
  // TODO: Check this as it should be restricted who can access this function

  void 
  reset(T* newPtr = nullptr) {
    delete m_ptr;
    m_ptr = newPtr;
  }

  // Method to create a WeakPointer
  SmallPtr<T> 
  ptr() {
    return SmallPtr<T>(*this);
  }

  template <typename U>
  friend SmartPtr<U> reinterpret_smart_cast(SmartPtr<T>&& uptr);

  friend class SmartPointers;
  friend class SmallPtr<T>;

private:
  
  T* get() const { return m_ptr; }

  T* m_ptr;
};

// reinterpret_pointer_cast function
template <typename U, typename T>
SmartPtr<U> reinterpret_smart_cast(SmartPtr<T>&& uptr) {
  // U* castedPtr = reinterpret_cast<U*>(uptr.get());
  // uptr.ptr = nullptr;  // Release the ownership from the original pointer
  // return SmartPtr<U>(castedPtr); // Return the new UniquePointer with casted type
  // TODO: Check iof this is the correct thing to do
  return *reinterpret_cast<SmartPtr<U>*>(&uptr);
}


class SmartPointers : public Module<SmartPointers> {

public:
  ~SmartPointers() {
    // Cleanup all pointers
    cleanup();
  }

  // Store a UniquePointer and return the raw pointer
  // Not really fond of this one but hey, the better way to store smart pointers the better
  template <typename T>
  T* 
  store(SmartPtr<T>&& uniquePtr) {
    SmartPtr<T>* newPtr = new SmartPtr<T>(std::move(uniquePtr));
    pointers.insert(newPtr);
    return newPtr->get();
  }

  // Garbage collection to clean up dangling pointers
  void 
  cleanup() {
    int32 size = 0;
    for (auto ptr : pointers) {
      if (ptr->m_counter <= 1) {
        delete ptr;
        ++size;
      }
    }
    // All pointers stop being used
    if (size == pointers.size()) {
      pointers.clear();
    }
  }

  template <typename T, typename... Args>
  SmartPtr<T> 
  create(Args ... args) {
    SmartPtr<T>* newPtr = new SmartPtr<T>(std::forward<Args>(args)...);
    pointers.insert(newPtr);
    return *newPtr;
  }
  
    
  // Optional: Remove a specific pointer if needed
  template <typename T>
  void 
  remove(T* rawPtr) {
    for (auto it = pointers.begin(); it != pointers.end(); ++it) {
      if (*it == rawPtr) {
        delete* it;
        pointers.erase(it);
        break;
      }
    }
  }

private:

  // TODO: Check if this should be raw or unique
  UnorderedSet<ManagedPtr*> pointers;

};


// FUNCTIONS

template <typename T, typename... Args>
UniquePointer<T> makeUniquePtr(Args ... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
SharedPointer<T> makeSharedPtr(Args ... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
SmartPtr<T> makeSmartPtr(Args ... args) {
  CY_ASSERT(!SmartPointers::isStarted() && "Smart Pointer Manager was not started");
  return SmartPointers::instance().create<T>(std::forward<Args>(args)...);
}

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
