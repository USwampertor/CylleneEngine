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
#include "cyBeing.h"
#include "cySmartPointers.h"

namespace CYLLENE_SDK
{

class ClassRegister
{
  using FabricatorFunc = Callback<SharedPointer<Being>>;

  static void registerBeing(const String& beingClassName, 
                            FabricatorFunc creator) {
    getBeingRegistry()[beingClassName] = creator;
  }

  template<class... Args>
  static SharedPointer<Being> createBeing(const String& beingClassName, 
                                          Args&& ...args) {
    auto it = getBeingRegistry().find(beingClassName);
    if (it != getBeingRegistry().end()) {
      return it->second(std::forward<Args>(args)...);
    }
  }

  template<typename T, class... Args>
  static SharedPointer<T> createBeing(const String& beingClassName,
                                      Args&&... args) {
    auto it = getBeingRegistry().find(beingClassName);
    if (it != getBeingRegistry().end()) {
      return REINTERPRETPOINTER(T, it->second(std::forward<Args>(args)...));
    }
  }

  static Map<String, FabricatorFunc>& getBeingRegistry() {
    static Map<String, FabricatorFunc> beingRegistry;
    return beingRegistry;
  }

};


#define REGISTER_CLASS(beingClassName) \
    namespace { \
        const bool registered_##beingClassName = (ClassRegister::registerBeing(#beingClassName, []() -> SharedPointer<Being> { return  MakeSharedObject<Being>(); }), true); \
    }



}

