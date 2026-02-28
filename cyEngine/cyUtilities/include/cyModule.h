/**
 * @file cyModule.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Module.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyUtilities.h"

namespace CYLLENE_SDK {

/**
 * @class Module
 * @brief Generic startup/shutdown singleton-style module wrapper.
 * @tparam T Concrete module type.
 */
template <class T>
class Module
{
 public:

  /**
   * @brief Returns a reference to the active module instance.
   * @return Active module reference.
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
   * @brief Returns a pointer to the active module instance.
   * @return Active module pointer.
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

  /**
   * @brief Starts the module using type `T`.
   * @tparam Args Constructor argument types.
   * @param args Constructor arguments.
   */
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

  /**
   * @brief Starts the module with a `T`-derived subtype.
   * @tparam SubType Concrete module subtype.
   * @tparam Args Constructor argument types.
   * @param args Constructor arguments.
   */
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

  /**
   * @brief Shuts down and destroys the module instance.
   */
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

  /**
   * @brief Checks whether module is running.
   * @return True if started and not destroyed.
   */
  static bool
  isStarted() {
    return isStartedUp() && !isDestroyed();
  }

  /**
   * @brief Injects an already-created module instance.
   * @param obj Module instance pointer.
   */
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
   * @brief Default constructor.
   */
  Module() = default;

  /**
   * @brief Virtual destructor.
   */
  virtual
  ~Module() = default;

  Module(Module&&) = delete;

  Module(const Module&) = delete;

  Module&
  operator=(Module&&) = delete;

  Module&
  operator=(const Module&) = delete;

  /**
   * @brief Called immediately after module startup.
   */
  virtual void
  onStartUp() {}

  /**
   * @brief Called immediately before module shutdown.
   */
  virtual void
  onShutDown() {}

  /**
   * @brief Internal storage for module instance pointer.
   * @return Reference to static instance pointer.
   */
  static T*&
  _instance() {
    static T* inst = nullptr;
    return inst;
  }

  /**
   * @brief Internal destroyed-state flag.
   * @return Reference to destroyed-state flag.
   */
  static bool&
  isDestroyed() {
    static bool inst = false;
    return inst;
  }

  /**
   * @brief Internal startup-state flag.
   * @return Reference to startup-state flag.
   */
  static bool&
  isStartedUp() {
    static bool inst = false;
    return inst;
  }
};
}

