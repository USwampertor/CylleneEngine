#pragma once

#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"
#include "cyVector2f.h"
#include "cyVector3f.h"
#include "cyVector4f.h"
#include "cyMatrix3.h"
#include "cyMatrix4.h"

namespace CYLLENE_SDK
{



  namespace GSHADERTYPE
  {
  BETTER_ENUM(E, int32,
              eUNKNOWN  = -1,
              eNONE     = 0,
              eVERTEX   = 1,
              ePIXEL    = 2,
              eGEOMETRY = 3,
              eCOMPUTE  = 4
  );
  }

  namespace GSHADERMODEL
  {
  BETTER_ENUM(E, uint32,
              eUNKNOWN = -1,
              eNONE = 0,
              eSM_4_0 = 1,
              eSM_4_1 = 2,
              eSM_5_0 = 3,
              eSM_5_1 = 4,
              eSM_6_0 = 5,
              eSM_6_1 = 6,
              eSM_6_2 = 7,
              eSM_6_3 = 8,
              eSM_6_4 = 9,
              eSM_6_5 = 10
  );
  }

  namespace GSHADERPARAM
  {
  BETTER_ENUM(E, uint32,
              eUNKNOWN = -1,
              eNONE = 0,
              eFLOAT = 1,
              eINT = 2,
              eUINT = 3,
              eVECTOR2 = 4,
              eVECTOR3 = 5,
              eVECTOR4 = 6,
              eMATRIX3 = 7,
              eMATRIX4 = 8,
              eTEXTURE2D = 9,
              eTEXTURECUBE = 10,
              eSAMPLERSTATE = 11
  );
  }

static uint32 sizeOfShaderParam(GSHADERPARAM::E type)
{
  switch (type) {
  case GSHADERPARAM::E::eFLOAT:    return sizeof(float);
  case GSHADERPARAM::E::eINT:      return sizeof(int32);
  case GSHADERPARAM::E::eUINT:     return sizeof(uint32);
  case GSHADERPARAM::E::eVECTOR2:  return sizeof(Vector2f);
  case GSHADERPARAM::E::eVECTOR3:  return sizeof(Vector3f);
  case GSHADERPARAM::E::eVECTOR4:  return sizeof(Vector4f);
  case GSHADERPARAM::E::eMATRIX3:  return sizeof(Matrix3);
  case GSHADERPARAM::E::eMATRIX4:  return sizeof(Matrix4);
  default:                         return 0;
  }
}

struct GShaderBlob
{
public:
  
  /*
   * The idea is that the DX11 Shaderblob has a D3D11Blob 
   */
  virtual void*
  getBlob() = 0;

  uint32 size;
  uint32 type;
  String entry;
  String model;
  String data;
  String error;
  bool isCompiled = false;
};

struct GShaderValue
{
  String name;
  GSHADERPARAM::E type = GSHADERPARAM::E::eUNKNOWN;
  uint32 count = 1;
  uint32 offset = 0;
};

class CY_GRAPHICS_EXPORT GShader : public GGraphic
{
public:
  GShader() = default;

  virtual ~GShader() {}
  
  virtual void*
  getBlob() = 0;

  virtual void*
  getShader() = 0;

  virtual void 
  reflect() = 0;

  bool 
  hasValue(const String& name, const GSHADERPARAM::E& type) {
    for (auto& val : m_values) {
      if (name  == val.name && type == val.type) {
        return true;
      }
    }
    return false;
  }

  Vector<GShaderValue>& getValues() { return m_values; }

  Vector<GShaderValue> m_values;
};


class GVertexShader : public GShader
{
public:
  GVertexShader() = default;
  virtual ~GVertexShader() = default;
};

class GPixelShader : public GShader
{
public:
  GPixelShader() = default;
  virtual ~GPixelShader() = default;
};

class GGeometryShader : public GShader
{
public:
  GGeometryShader() = default;
  virtual ~GGeometryShader() = default;
};

class GComputeShader : public GShader
{
public:
  GComputeShader() = default;
  virtual ~GComputeShader() = default;
};

}