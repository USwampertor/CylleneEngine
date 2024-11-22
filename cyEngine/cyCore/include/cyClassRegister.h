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
#include "cySmartPointers.h"

#include <iostream>

namespace CYLLENE_SDK
{

class Being;

class CY_CORE_EXPORT ClassRegister
{
public:
  using FabricatorFunc = std::function<SharedPointer<Being>()>;

  static void registerBeing(const String& beingClassName, 
                            FabricatorFunc creator);

  template<class... Args>
  static SharedPointer<Being> createBeing(const String& beingClassName, 
                                          Args&& ...args);

  template<typename T, class... Args>
  static SharedPointer<T> createBeing(const String& beingClassName,
                                      Args&&... args);

  static Map<String, FabricatorFunc>& getBeingRegistry() {
    static Map<String, FabricatorFunc> m_beingRegistry;
    return m_beingRegistry;
  }
};


#define REGISTER_CLASS(beingClassName) \
    namespace { \
        const bool registered_##beingClassName = (ClassRegister::registerBeing(#beingClassName, []() -> SharedPointer<Being> { return  MakeSharedObject<Being>(); }), true); \
    }

// Define as much variables that should exist in all Being classes
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

