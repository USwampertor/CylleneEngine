#include "cyRMaterialInstance.h"
#include <cyUtilities.h>

namespace CYLLENE_SDK {

void
RMaterialInstance::setMaterial(const SPtr<RMaterial>& material) {
  m_material = material;

  m_overrides.clear();
  m_overrides = material->getDefaultValues();
}

void
RMaterialInstance::setValue(const String& name, MaterialValue value) {
  m_overrides[name] = value;
}

void
RMaterialInstance::clearValue(const String& name) {
  auto it = m_overrides.find(name);
  if (it != m_overrides.end()) m_overrides.erase(it);
}

void
RMaterialInstance::clearAllValues() {
  m_overrides.clear();
}

bool
RMaterialInstance::hasValue(const String& name) const {
  return m_overrides.find(name) != m_overrides.end();
}

MaterialValue
RMaterialInstance::getValue(const String& name) const {
  auto it = m_overrides.find(name);
  if (it != m_overrides.end()) return it->second;
  return nullptr;
}

MaterialValue
RMaterialInstance::getResolvedValue(const String& name) const {
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
