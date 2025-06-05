/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	cyClassRegister.h
 * @author 	Marco "Swampy" Millan
 * @date 	2024/11/20
 * @brief 	
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCorePrerequisites.h"
#include <cySmartPointers.h>

#include <iostream>

namespace CYLLENE_SDK
{

class BBeing;

class CY_CORE_EXPORT ClassRegister
{
public:
  using FabricatorFunc = std::function<SPtr<BBeing>()>;

  static void registerBeing(const String& beingClassName, 
                            FabricatorFunc creator);

  template<class... Args>
  static SPtr<BBeing> createBeing(const String& beingClassName,
                                          Args&& ...args) {
    auto it = getBeingRegistry().find(beingClassName);
    if (it != getBeingRegistry().end()) {
      return it->second(std::forward<Args>(args)...);
    }
    CY_ASSERT(false, "Being class not registered: " + beingClassName);
    return nullptr;
  }

  template<typename T,
           typename = std::enable_if_t<std::is_base_of<BBeing, T>::value>,
           class... Args>
  static SPtr<T> createBeing(Args&&... args) {
    auto it = getBeingRegistry().find(T::getClassName());
    if (it != getBeingRegistry().end()) {
      return REINTERPRETPOINTER(T, it->second(std::forward<Args>(args)...));
    }
  }

  static Map<String, FabricatorFunc>& getBeingRegistry() {
    static Map<String, FabricatorFunc> m_beingRegistry;
    return m_beingRegistry;
  }
};




#define REGISTER_CLASS(beingClassName) \
    namespace { \
        const bool registered_##beingClassName = \
          (ClassRegister::registerBeing(#beingClassName, []() -> SPtr<BBeing> { return  makeSharedPtr<BBeing>(); }), true); \
    }

// Define as much variables that should exist in all BBeing classes
#define BODY()                                                                \
public:                                                                       \
    static const String getClassName() {                                      \
        String fullFunction = __PRETTY_FUNCTION__;                            \
        String prefix = "CYLLENE_SDK::";                                      \
        auto start = fullFunction.find(prefix);                               \
        auto end = fullFunction.find("::getClassName");                       \
        String toReturn = fullFunction.substr(start + prefix.size(),          \
                                              end - (start + prefix.size())); \
        return toReturn;                                                      \
    }

}

