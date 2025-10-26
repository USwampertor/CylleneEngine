#include "cyCMaterialInstance.h"
#include <cyUtilities.h>

namespace CYLLENE_SDK {

const String
CMaterialInstance::toString() {
  String baseName = "<none>";
  if (auto mat = m_material.lock()) {
    baseName = mat->getName();
  }
  return Utils::format("Base: %s, Overrides: %zu",
                       baseName.c_str(),
                       static_cast<size_t>(m_overrides.size()));
}

void
CMaterialInstance::setMaterial(const SPtr<RMaterial>& material) {
  m_material = material;

  m_overrides.clear();
  m_overrides = material->getDefaultValues();
}

void
CMaterialInstance::setValue(const String& name, MaterialValue value) {
  m_overrides[name] = value;
}

void
CMaterialInstance::clearValue(const String& name) {
  auto it = m_overrides.find(name);
  if (it != m_overrides.end()) m_overrides.erase(it);
}

void
CMaterialInstance::clearAllValues() {
  m_overrides.clear();
}

bool
CMaterialInstance::hasValue(const String& name) const {
  return m_overrides.find(name) != m_overrides.end();
}

MaterialValue
CMaterialInstance::getValue(const String& name) const {
  auto it = m_overrides.find(name);
  if (it != m_overrides.end()) return it->second;
  return nullptr;
}

MaterialValue
CMaterialInstance::getResolvedValue(const String& name) const {
  // Prefer override if present
  auto it = m_overrides.find(name);
  if (it != m_overrides.end()) return it->second;

  // Fallback to base material defaults
  if (auto mat = m_material.lock()) {
    const auto& defaults = mat->getDefaultValues();
    auto jt = defaults.find(name);
    if (jt != defaults.end()) return jt->second;
  }
  return nullptr;
}

}
