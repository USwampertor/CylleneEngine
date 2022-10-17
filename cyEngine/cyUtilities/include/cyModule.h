/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyModule.h
 * @author Marco "Swampy" Millan
 * @date 8/4/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once
#include "cyUtilitiesPrerequisites.h"
#include "cyUtilities.h"

namespace CYLLENE_SDK {

/**
  * @class cyModule
  * @brief An Engine Module that can be started up and shut down manually
  *        Its a nicely done singleton, (I CANT BELIEVE ITS NOT SINGLETON!)
  */
template <class T>
class Module
{
 public:

  /**
    * Returns a reference to the instance
    */
  static T&
  instance() {
    if (!isStartedUp()) {
      Utils::throwException("Trying to access a module but it hasn't been started.");
    }

    if (isDestroyed()) {
      Utils::throwException("Trying to access a destroyed module.");
    }

    return *_instance();
  }

  /**
    * Returns a pointer to the instance
    */
  static T*
  instancePtr() {
    if (!isStartedUp()) {
      Utils::throwException("Trying to access a module but it hasn't been started.");
    }

    if (isDestroyed()) {
      Utils::throwException("Trying to access a destroyed module.");
    }

    return _instance();
  }


  template<class... Args>
  static void
  startUp(Args&& ...args) {
    if (isStartedUp()) {
      Utils::throwException("Trying to start an already started module.");
    }

    _instance() = cy_new<T>(std::forward<Args>(args)...);
    isStartedUp() = true;

    static_cast<Module*>(_instance())->onStartUp();
  }

  template<class SubType, class... Args>
  static void
  startUp(Args&& ...args) {
    static_assert(std::is_base_of<T, SubType>::value,
      "Provided type isn't derived from type the Module is initialized with.");

    if (isStartedUp()) {
      Utils::throwException("Trying to start an already started module.");
    }

    _instance() = cy_new<SubType>(std::forward<Args>(args)...);
    if (nullptr == _instance()) {
      Utils::throwException("Instance failed to initialize");
    }
    isStartedUp() = true;

    static_cast<Module*>(_instance())->onStartUp();
  }

  static void
  shutDown() {
    if (isDestroyed()) {
      Utils::throwException("Trying to shut down an already shut down module.");
    }

    if (!isStartedUp()) {
      Utils::throwException("Trying to shut down a module which was never started.");
    }

    static_cast<Module*>(_instance())->onShutDown();

    cy_delete(_instance());
    isDestroyed() = true;
  }

  static bool
  isStarted() {
    return isStartedUp() && !isDestroyed();
  }

  static void
  setModule(T* obj) {
    _instance() = obj;
    if (nullptr == _instance()) {
      Utils::throwException("Instance failed to be set");
    }
    isStartedUp() = true;
  }

protected:

  /**
    * Default constructor
    */
  Module() = default;

  /**
    * Virtual destructor
    */
  virtual
  ~Module() = default;

  Module(Module&&) = delete;

  Module(const Module&) = delete;

  Module&
  operator=(Module&&) = delete;

  Module&
  operator=(const Module&) = delete;

  virtual void
  onStartUp() {}

  virtual void
  onShutDown() {}

  static T*&
  _instance() {
    static T* inst = nullptr;
    return inst;
  }

  static bool&
  isDestroyed() {
    static bool inst = false;
    return inst;
  }

  static bool&
  isStartedUp() {
    static bool inst = false;
    return inst;
  }
};
}

