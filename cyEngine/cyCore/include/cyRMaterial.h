/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	cyMaterial.h
 * @author 	Marco "Swampy" Millan
 * @date 	  2024/11/15
 * @brief 	Material reflects the information from a defined shader
 *          and exposes it so it can be modified and binded to specific values,
 *          objects, types, etc.
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCorePrerequisites.h"

#include "cyMatrix3.h"
#include "cyMatrix4.h"
#include "cyRResource.h"
#include "cyRShader.h"
#include "cyRTexture.h"
#include "cyVector2f.h"
#include "cyVector3f.h"
#include "cyVector4f.h"
#include <cyColor.h> 

namespace CYLLENE_SDK {


  namespace TEXTURE_CHANNEL
  {
    BETTER_ENUM(E, uint32, 
      eCOLOR, 
      eNORMAL, 
      eROUGHNESS, 
      eMETALLIC,
      eEMISSIVE, 
      eOCCLUSION,
      eOPACITY);
  }

// struct _declspec(align(16)) MaterialData
// {
//   Color baseColor = Color::WHITE;
//   Color emissiveColor = Color::BLACK;
//   float normalScale = 1.0f;
//   float roughness = 1.0f;
//   float metallic = 0.0f;
//   float opacity = 1.0f;
//   float opacityThreshold = 0.5f;
//   float occlusion = 1.0f;
//   float padding[2];
// };
// 
// struct MaterialSignature
// {
//   uint32 bIsUsingBaseColor : 1;
//   uint32 bIsUsingBaseColorTexture : 1;
//   
//   uint32 bIsUsingNormalScale : 1;
//   uint32 bIsUsingNormalTexture : 1;
//   
//   uint32 bIsUsingEmissiveColor : 1;
//   uint32 bIsUsingEmissiveColorTexture : 1;
//   
//   
//   uint32 bIsUsingRoughnessTexture : 1;
//   uint32 bIsUsingMetallicTexture : 1;
//   uint32 bIsUsingEmissiveTexture : 1;
//   uint32 bIsUsingOpacityTexture : 1;
//   uint32 bIsUsingOpacity : 1;
//   uint32 bIsUsingOpacityThreshold : 1;
//   uint32 shaderHash = 0;
//   uint32 textureHashes[TEXTURE_CHANNEL::E::_size()] = { 0 };
// };

using MaterialValue = void*; // could be float, vec2, vec3, vec4, mat3, mat4, texture, etc.
  
/*
 *	@class RMaterial 	
 *	@brief RMaterial reflects the information from a defined base shader and sets a 
 *  series of values that can be used to modify the shader at runtime.
 *
 */
class CY_CORE_EXPORT RMaterial : public RResource
{
public:
  RMaterial() : RResource(RMaterial::staticType()) {}

  ~RMaterial() = default;

  static RESOURCE_TYPE::E staticType() { return RESOURCE_TYPE::E::eMATERIAL; }

  virtual void*
  getData() override;

  virtual void
  setData(void* data) override;

  void
  setBaseShader(const SPtr<RShader>& shader) { m_shader = shader; }

  WPtr<RShader>
  getBaseShader() const { return m_shader; }

  const UnorderedMap<String, MaterialValue>&
  getDefaultValues() const { return m_values; }

private:

  WPtr<RShader> m_shader;
  UnorderedMap<String, MaterialValue> m_values;

};

}

