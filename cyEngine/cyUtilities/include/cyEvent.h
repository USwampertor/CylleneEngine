/**
 * @file cyEvent.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Event.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyUtilities.h"
#include <functional>
#include <utility> // Add this for std::forward

namespace CYLLENE_SDK
{

template<typename ReturnType, typename... Args>
using Callback = std::function<ReturnType(Args...)>;

/**
 * @class Event
 * @brief Lightweight callback list with add/remove/invoke operations.
 * @tparam ReturnType Callback return type.
 * @tparam Args Callback argument types.
 */
template<typename ReturnType, typename... Args>
class Event
{
public:

  Event() = default;

  ~Event() = default;

  /**
   * @brief Adds a listener callback.
   * @param newFunction Callback to register.
   */
  void
  addListener(const Callback<ReturnType, Args...>& newFunction) {
    m_events.push_back(newFunction);
  }

  /**
   * @brief Invokes all listeners with forwarded arguments.
   * @param args Arguments forwarded to each listener.
   */
  void 
  invoke(Args&&... args) {
    for (auto& e : m_events) {
      e(std::forward<Args>(args)...);
    }
  }

  /**
   * @brief Removes a specific listener callback.
   * @param newFunction Callback to remove.
   */
  void
  removeListener(const Callback<ReturnType, Args...>& newFunction) {
    m_events.erase(std::remove(m_events.begin(), m_events.end(), newFunction), m_events.end());
  }

  /**
   * @brief Removes all registered listeners.
   */
  void 
  removeAllListeners() {
    m_events.clear();
  }

  /**
   * @brief Adds a listener using operator syntax.
   * @param newFunction Callback to register.
   */
  void
  operator+=(const Callback<ReturnType, Args...>& newFunction) {
    addListener(newFunction);
  }

  /**
   * @brief Removes a listener using operator syntax.
   * @param newFunction Callback to remove.
   */
  void
  operator-=(const Callback<ReturnType, Args...>& newFunction) {
    removeListener(newFunction);
  }

private:
  /**
   * @brief Registered listeners.
   */
  Vector<Callback<ReturnType, Args...>> m_events;

};
}
