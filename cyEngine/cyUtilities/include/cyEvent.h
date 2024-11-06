#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyUtilities.h"
#include <functional>

namespace CYLLENE_SDK
{

template<typename ReturnType, typename... Args>
using Callback = std::function<ReturnType(Args...)>;


template<typename ReturnType, typename... Args>
class Event
{
public:

  Event() = default;

  ~Event() = default;

  void
  addListener(const Callback<ReturnType, Args...>& newFunction)
  {
    m_events.push_back(newFunction);
  }

  void 
  invoke(Args... args)
  {
    for (auto& e : m_events)
    {
      e(std::forward(args...));
    }
  }

  void
  removeListener(const Callback<ReturnType, Args...>& newFunction)
  {
    m_events.erase(std::remove(m_events.begin(), m_events.end(), newFunction), m_events.end());
  }

  void 
  removeAllListeners()
  {
    m_events.clear();
  }

  void
  operator+=(const Callback<ReturnType, Args...>& newFunction)
  {
    addListener(newFunction);
  }

  void
  operator-=(const Callback<ReturnType, Args...>& newFunction)
  {
    removeListener(newFunction);
  }

private:
  Vector<Callback<ReturnType, Args...>> m_events;

};
}


