/************************************************************************/
/**
 * @file chTypeTraits.h
 * @author AccelMR
 * @date 2025/07/12
 * @brief  Asset type traits for Chimera Core.
 */
 /************************************************************************/
#pragma once

#include "cyPlatformTypes.h"
#include "cyUUID.h"

namespace CYLLENE_SDK {

template <typename T> struct TypeTraits {
  static constexpr const ANSICHAR*
    getTypeName() {
    return "Unknown";
  }
  static const UUID&
    getTypeId() {
    return UUID::null();
  }
};

} // namespace CYLLENE_SDK

#define DECLARE_TYPE_TRAITS(TypeClass)                                                      \
template <> struct CYLLENE_SDK::TypeTraits<TypeClass> {                                            \
  static constexpr const CYLLENE_SDK::ANSICHAR*                                                    \
  getTypeName() {                                                                           \
    return #TypeClass;                                                                      \
  }                                                                                         \
  static const CYLLENE_SDK::UUID&                                                                  \
  getTypeId() {                                                                             \
    static const CYLLENE_SDK::UUID typeId = CYLLENE_SDK::UUID::createFromName(#TypeClass);                \
    return typeId;                                                                          \
  }                                                                                         \
};

#define DECLARE_TYPE_TRAITS_NAMESPACE_ID(NameSpaceIdExpr, TypeClass)                        \
template <> struct CYLLENE_SDK::TypeTraits<TypeClass> {                                            \
  static constexpr const CYLLENE_SDK::ANSICHAR*                                                    \
  getTypeName() {                                                                           \
    return #TypeClass;                                                                      \
  }                                                                                         \
  static const CYLLENE_SDK::UUID&                                                                  \
  getTypeId() {                                                                             \
    static const CYLLENE_SDK::UUID typeId = CYLLENE_SDK::UUID::createFromName(#TypeClass, NameSpaceIdExpr); \
    return typeId;                                                                          \
  }                                                                                         \
  };
