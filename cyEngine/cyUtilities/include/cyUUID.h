#pragma once
#include "cyPlatformDefines.h"
#include "cyStdHeaders.h"
#include "cyPlatformTypes.h"

#include <stduuid/uuid.h>
namespace CYLLENE_SDK
{
/**
  * @brief Canonical on-disk size of a serialized UUID, in bytes.
  *
  * A UUID always serializes to its raw 128-bit value (see @ref UUID::toBytes),
  * independent of @c sizeof(UUID) in memory. Use this — never @c sizeof(UUID) —
  * to size on-disk layouts.
  */
constexpr size_t kUuidBytes = 16;

class CY_UTILITY_EXPORT UUID {
 public:
  UUID() = default;

  //explicit UUID(Stringview str);

  NODISCARD FORCEINLINE bool
  isNull() const { return m_uuid.is_nil(); }

  NODISCARD FORCEINLINE String
  toString() const { return uuids::to_string(m_uuid); }

  NODISCARD FORCEINLINE bool
  operator==(const UUID& other) const { return m_uuid == other.m_uuid; }

  NODISCARD FORCEINLINE bool
  operator!=(const UUID& other) const { return m_uuid != other.m_uuid; }

  NODISCARD FORCEINLINE bool
  operator<(const UUID& other) const { return m_uuid < other.m_uuid; }

  NODISCARD FORCEINLINE size_t
  getHash() const noexcept { return Hash<uuids::uuid>{}(m_uuid); }

  /** @brief The raw 16-byte value, for compact binary serialization. */
  NODISCARD Array<uint8, kUuidBytes>
  toBytes() const;

  /** @brief Rebuild a UUID from its raw 16 bytes (see @ref toBytes). */
  NODISCARD static UUID
  fromBytes(const Array<uint8, kUuidBytes>& bytes);

  NODISCARD static UUID
  createRandom();

  NODISCARD static UUID
  createFromName(Stringview name, const UUID& namespace_uuid = UUID::null());

  NODISCARD FORCEINLINE static const UUID&
  null() { static UUID nullUUID(uuids::uuid{}); return nullUUID; }

 protected:

  explicit UUID(const uuids::uuid& uuid)
    : m_uuid(uuid) {}

 private:
  friend Hash<UUID>;
  uuids::uuid m_uuid;

  static const String CY_UUID_STR;
  static uuids::uuid_name_generator UUID_NAME_GENERATOR;
};

} // namespace CYLLENE_SDK

namespace std {
template <>
struct hash<CYLLENE_SDK::UUID> {
  using argument_type = CYLLENE_SDK::UUID;
  using result_type = CYLLENE_SDK::SizeT;

  NODISCARD result_type
    operator()(const argument_type& uuid) const noexcept {
    return uuid.getHash();
  }

}; // hash
} // namespace std


