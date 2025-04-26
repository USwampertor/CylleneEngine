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
#include "cyResource.h"
#include "cyShader.h"
#include "cyVector2f.h"
#include "cyVector3f.h"
#include "cyVector4f.h"

namespace CYLLENE_SDK {

class CY_CORE_EXPORT MaterialResource : public Resource
{
public:
  MaterialResource() : Resource(MaterialResource::staticType()) {}

  static RESOURCE_TYPE::E staticType() { return RESOURCE_TYPE::E::eMATERIAL; }

  virtual void*
  getData() override;

  virtual void
  setData(void* data) override;

  /*
   *	@brief	Sets a value in the material, setting it as well in the shader
   *	@param	const String& flag the flag to find in the material, referenced in the shader
   *	@param	const T& value, the value to set in the material. This T type should
   *          be compatible with the defined type inside the shader
   */
  template <typename T>
  void
  setValue(const String& flag, const T& newValue);

  void
  setFloat(const String& flag, const float& newValue);

  void
  setInt32(const String& flag, const int32& newValue);

  void
  setUint32(const String& flag, const uint32& newValue);

  void
  setVector2(const String& flag, const Vector2f& newValue);

  void
  setVector3(const String& flag, const Vector3f& newValue);

  void
  setVector4(const String& flag, const Vector4f& newValue);

  void 
  setMatrix3(const String& flag, const Matrix3& newValue);

  void 
  setMatrix4(const String& flag, const Matrix4& newValue);

  /*
   *	@brief	Retrieves a value from the material
   *	@param	const String& flag the flag to find in the material, referenced in the shader
   *  @return T& value defined inside the shader (This can be a float, Vector, Texture, etc)	
   */
  template <typename T>
  T&
  getValue(const String& flag);

  /*
   *	@brief	Retrieves a float from the material
   *	@param	const String& flag the flag to find in the material, referenced in the shader
   *  @return float& value defined inside the shader
   */
  float&
  getFloat(const String& flag);

  /*
   *	@brief	Retrieves a int32 from the material
   *	@param	const String& flag the flag to find in the material, referenced in the shader
   *  @return int32& value defined inside the shader
   */
  int32&
  getInt32(const String& flag);

  /*
   *	@brief	Retrieves a uint32 from the material
   *	@param	const String& flag the flag to find in the material, referenced in the shader
   *  @return uint32& value defined inside the shader
   */
  uint32&
  getUint32(const String& flag);

  /*
   *	@brief	Retrieves a Vector2 from the material
   *	@param	const String& flag the flag to find in the material, referenced in the shader
   *  @return Vector2f& value defined inside the shader
   */
  Vector2f&
  getVector2(const String& flag);

  /*
   *	@brief	Retrieves a Vector3f from the material
   *	@param	const String& flag the flag to find in the material, referenced in the shader
   *  @return Vector3f& value defined inside the shader
   */
  Vector3f&
  getVector3(const String& flag);

  /*
   *	@brief	Retrieves a Vector4f from the material
   *	@param	const String& flag the flag to find in the material, referenced in the shader
   *  @return Vector4f& value defined inside the shader
   */
  Vector4f&
  getVector4(const String& flag);

  /*
   *	@brief	Retrieves a Matrix3 from the material
   *	@param	const String& flag the flag to find in the material, referenced in the shader
   *  @return Matrix3& value defined inside the shader
   */
  Matrix3& 
  getMatrix3(const String& flag);

  /*
   *	@brief	Retrieves a Matrix4 from the material
   *	@param	const String& flag the flag to find in the material, referenced in the shader
   *  @return Matrix4& value defined inside the shader
   */
  Matrix4& getMatrix4(const String& flag);

  /*
   *	@brief	retrieves a new shader, getting its values via reflection and making them
   *          manipulable to the material
   *	@param	const SPtr<ShaderResource>& newShader the new shader to set
   */
  void
  loadShader(const SPtr<ShaderResource>& newShader);

  
private:

  void
  setShaderValues();

  void
  getShaderValues();

private:

  Map<String, SPtr<void*>> m_components;

  WPtr<ShaderResource> m_shader;
};

}

