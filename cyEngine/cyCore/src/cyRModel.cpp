#include "cyRModel.h"

#include <cyJSON.h>
#include <cyResourceManager.h>
#include <cyRMesh.h>

/*

{
name:foo
information:bar
meshes:[
{
vertices:[
{
position:[x,y,z],
uv:[x,y],
normal:[x,y,z],
tangent:[x,y,z],
binormal:[x,y,z],
color:[r,g,b,a],
boneIDs:[1,2,3,4],
boneWeights:[f,f,f,f],
customData:[1,2,3,4]
}
...
],
indices:[...],
material:gar
},
...
]
bones:{},
animations{},
...
}
*/

namespace CYLLENE_SDK {
void*
RModel::getData() {
  return nullptr;
}

void
RModel::setData(void* data) {
  JSONDocument d;
  String dataString = *reinterpret_cast<String*>(data);
  d.Parse(dataString);
  uint32 subMeshIndex = 0;
    
  // Meshes
  for (auto& mesh : d["meshes"].GetArray()) {
    String subMeshName = Utils::format("%s_sub%d", m_name.c_str(), subMeshIndex);
    m_meshes.push_back(ResourceManager::instance().create<RMesh>(subMeshName));

      
    // Vertices
    for (auto& vertex : mesh["vertices"].GetArray()) {
      Vertex v;

      // Position
      uint32 i = 0;
      for (auto& posAxis : vertex["position"].GetArray()) {
        v.m_position[i] = posAxis.GetFloat();
        ++i;
      }

      // Normal
      i = 0;
      for (auto& normalAxis : vertex["normal"].GetArray()) {
        v.m_normal[i] = normalAxis.GetFloat();
        ++i;
      }

      // Tangent
      i = 0;
      for (auto& tangentAxis : vertex["tangent"].GetArray()) {
        v.m_tangent[i] = tangentAxis.GetFloat();
        ++i;
      }

      // Binormal
      i = 0;
      for (auto& binormalAxis : vertex["binormal"].GetArray()) {
        v.m_binormal[i] = binormalAxis.GetFloat();
        ++i;
      }

      // Color
      i = 0;
      for (auto& colorAxis : vertex["color"].GetArray()) {
        v.m_color[i] = colorAxis.GetFloat();
        ++i;
      }

      // UV
      i = 0;
      for (auto& uvAxis : vertex["uv"].GetArray()) {
        v.m_uv[i] = uvAxis.GetFloat();
        ++i;
      }

      // BoneIDs
      i = 0;
      for (auto& boneID : vertex["boneIDs"].GetArray()) {
        v.m_boneIDs[i] = boneID.GetFloat();
        ++i;
      }

      // BoneWeights
      i = 0;
      for (auto& boneWeight : vertex["boneWeights"].GetArray()) {
        v.m_boneWeights[i] = boneWeight.GetFloat();
        ++i;
      }

      // CustomData
      i = 0;
      for (auto& customData : vertex["customData"].GetArray()) {
        v.m_customData[i] = customData.GetFloat();
        ++i;
      }

      m_meshes.back()->m_vertexBuffer.push_back(v);
    }

    for (auto& index : mesh["indices"].GetArray()) {
      m_meshes.back()->m_indexBuffer.push_back(index.GetInt());
    }


    // Finished with mesh number subMeshIndex
    ++subMeshIndex;
  }
  
  
  delete(data);
}
}