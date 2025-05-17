#include "cyClassRegister.h"

#include "cyBBeing.h"

namespace CYLLENE_SDK {

void
ClassRegister::registerBeing(const String& beingClassName, FabricatorFunc creator) {
  getBeingRegistry()[beingClassName] = creator;
}

// template<class... Args>
// SPtr<Being> 
// ClassRegister::createBeing(const String& beingClassName,
//                            Args&& ...args) {
//   auto it = getBeingRegistry().find(beingClassName);
//   if (it != getBeingRegistry().end()) {
//     return it->second(std::forward<Args>(args)...);
//   }
// }
// 
// template<typename T, class... Args>
// SPtr<T> 
// ClassRegister::createBeing(const String& beingClassName,
//                            Args&&... args) {
//   auto it = getBeingRegistry().find(beingClassName);
//   if (it != getBeingRegistry().end()) {
//     return REINTERPRETPOINTER(T, it->second(std::forward<Args>(args)...));
//   }
// }

}