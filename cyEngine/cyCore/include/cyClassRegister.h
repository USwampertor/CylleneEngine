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
#define BODY()                                                      \
public:                                                             \
    static std::string getClassName() {                             \
        std::string fullFunction = __PRETTY_FUNCTION__;             \
        auto start = fullFunction.find("CYLLENE_SDK::") + 1;                    \
        auto end = fullFunction.find("::getClassName");             \
        std::string toReturn = fullFunction.substr(start, end - start); \
        return toReturn; \
    }

}

