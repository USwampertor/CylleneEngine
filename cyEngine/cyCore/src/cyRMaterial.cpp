#include "cyRMaterial.h"
#include <cyJSON.h>
#include <cyResourceManager.h>
#include <cyRTexture.h>
#include <cyRShader.h>

namespace CYLLENE_SDK {

void*
RMaterial::getData() {
  return nullptr;
}

void
RMaterial::setData(void* data) {
  if (!data) return;
  String json = *reinterpret_cast<String*>(data);
  JSONDocument d;
  d.Parse(json);
  if (d.HasParseError()) return;
  if (!d.HasMember("type") || String(d["type"].GetString()) != String("material")) return;

  if (d.HasMember("baseShader") && d["baseShader"].IsString()) {
    String shaderName = d["baseShader"].GetString();
    SPtr<RShader> shader = ResourceManager::instance().get<RShader>(shaderName);
    if (shader) m_shader = shader;
  }

  if (d.HasMember("values") && d["values"].IsObject()) {
    const auto& vals = d["values"].GetObject();
    for (auto it = vals.MemberBegin(); it != vals.MemberEnd(); ++it) {
      String name = it->name.GetString();
      if (it->value.IsString()) {
        String texName = it->value.GetString();
        SPtr<RTexture> tex = ResourceManager::instance().get<RTexture>(texName);
        if (tex) {
          m_values[name] = tex.get();
        }
      }
    }
  }
}


}
