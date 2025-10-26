#pragma once
#include "cyCorePrerequisites.h"
#include "cyCComponent.h"
#include "cyRMaterial.h"
#include "cyResourceManager.h"

namespace CYLLENE_SDK
{

// CMaterialInstance represents a per-entity editable instance
// that references a base RMaterial and carries per-instance
// overrides for named material values.
class CY_CORE_EXPORT CMaterialInstance : public CComponent
{
public:
  CMaterialInstance(const SPtr<RMaterial>& material = nullptr)
    : CComponent(CMaterialInstance::staticType()), m_material(material) {}

  ~CMaterialInstance() = default;

  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eMATERIALINSTANCE; }

  // Summary text for inspectors/debugging
  virtual const String toString() override;

  // Base material reference
  void 
  setMaterial(const SPtr<RMaterial>& material);
  
  WPtr<RMaterial> getMaterial() const { return m_material; }

  // Per-instance overrides
  void 
  setValue(const String& name, MaterialValue value);
  
  template <typename T>
  void setValue(const String& name, const T& value) {
    setValue(name, reinterpret_cast<MaterialValue>(new T(value)));
  }
  
  void 
  clearValue(const String& name);

  void 
  clearAllValues();
  
  bool 
  hasValue(const String& name) const;
  
  MaterialValue 
  getValue(const String& name) const;              // override or nullptr
  
  MaterialValue 
  getResolvedValue(const String& name) const;      // override or base default

  const UnorderedMap<String, MaterialValue>& getOverrides() const { return m_overrides; }

private:
  WPtr<RMaterial> m_material;
  UnorderedMap<String, MaterialValue> m_overrides;
};

}
