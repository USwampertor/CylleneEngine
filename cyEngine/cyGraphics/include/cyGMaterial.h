#pragma once

#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"
#include "cyGShader.h"

#include <cyRMaterial.h>

namespace CYLLENE_SDK {

class GMaterial : public GGraphic
{
public:
  GMaterial() = default;
  ~GMaterial() = default;

  bool initialize();

  GShader* GetShader() const { return m_shader.lock().get(); }

  const MaterialValue* GetDefaultValue(const std::string& name) const;

  void SetDefaultValue(const std::string& name, const MaterialValue& value);

private:
  
  WPtr<RMaterial> m_material;
  WPtr<GShader> m_shader;
  std::unordered_map<std::string, MaterialValue> m_defaultValues;


};

}

