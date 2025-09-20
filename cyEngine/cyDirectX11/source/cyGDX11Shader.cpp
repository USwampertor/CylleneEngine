#include "cyGDX11Shader.h"
#include <d3d11shader.h>
#include <Winerror.h>
#include <d3dcompiler.h>

namespace CYLLENE_SDK
{

GDX11VertexShader::~GDX11VertexShader()
{
  DX11_SAFE_RELEASE(m_pVertexShader);
  DX11_SAFE_RELEASE(m_pBlob->m_pBlob);
}

void* 
GDX11VertexShader::getShader() {
  return m_pVertexShader;
}

void 
GDX11VertexShader::set(void* pHandle) {
  m_pVertexShader = static_cast<ID3D11VertexShader*>(pHandle);
}

void* 
GDX11VertexShader::get() {
  return m_pVertexShader;
}

void
GDX11VertexShader::reflect() {
  // Use D3DReflect to reflect the shader and get information about its inputs, outputs, and resources
  if (m_pBlob && m_pBlob->m_pBlob) {
    ID3D11ShaderReflection* pReflector = nullptr;
    HRESULT hr = D3DReflect(m_pBlob->m_pBlob->GetBufferPointer(),
                                 m_pBlob->m_pBlob->GetBufferSize(),
                                 IID_ID3D11ShaderReflection,
                                 reinterpret_cast<void**>(&pReflector));
    if (SUCCEEDED(hr)) {
      D3D11_SHADER_DESC shaderDesc;
      pReflector->GetDesc(&shaderDesc);

      // Clear previous reflection values
      m_values.clear();

      // Helper to map D3D type to engine GSHADERPARAM
      auto mapType = [](const D3D11_SHADER_TYPE_DESC& tdesc) -> GSHADERPARAM::E {
        if (tdesc.Class == D3D_SVC_SCALAR) {
          if (tdesc.Type == D3D_SVT_FLOAT) return GSHADERPARAM::E::eFLOAT;
          if (tdesc.Type == D3D_SVT_INT)   return GSHADERPARAM::E::eINT;
          if (tdesc.Type == D3D_SVT_UINT)  return GSHADERPARAM::E::eUINT;
        }
        else if (tdesc.Class == D3D_SVC_VECTOR) {
          // Assume float vectors; engine enum is untyped for vectors
          if (tdesc.Columns == 2) return GSHADERPARAM::E::eVECTOR2;
          if (tdesc.Columns == 3) return GSHADERPARAM::E::eVECTOR3;
          if (tdesc.Columns == 4) return GSHADERPARAM::E::eVECTOR4;
        }
        else if (tdesc.Class == D3D_SVC_MATRIX_ROWS || tdesc.Class == D3D_SVC_MATRIX_COLUMNS) {
          // Rows/Columns describe size; support 3x3 and 4x4
          if ((tdesc.Rows == 4 && tdesc.Columns == 4) || (tdesc.Rows == 1 && tdesc.Columns == 16))
            return GSHADERPARAM::E::eMATRIX4;
          if ((tdesc.Rows == 3 && tdesc.Columns == 3) || (tdesc.Rows == 1 && tdesc.Columns == 9))
            return GSHADERPARAM::E::eMATRIX3;
        }
        return GSHADERPARAM::E::eUNKNOWN;
      };

      // Reflect constant buffers and variables
      for (UINT cbIndex = 0; cbIndex < shaderDesc.ConstantBuffers; ++cbIndex) {
        ID3D11ShaderReflectionConstantBuffer* pCB = pReflector->GetConstantBufferByIndex(cbIndex);
        if (!pCB) continue;

        D3D11_SHADER_BUFFER_DESC cbDesc;
        if (FAILED(pCB->GetDesc(&cbDesc))) continue;

        for (UINT varIndex = 0; varIndex < cbDesc.Variables; ++varIndex) {
          ID3D11ShaderReflectionVariable* pVar = pCB->GetVariableByIndex(varIndex);
          if (!pVar) continue;
          D3D11_SHADER_VARIABLE_DESC varDesc;
          if (FAILED(pVar->GetDesc(&varDesc))) continue;

          ID3D11ShaderReflectionType* pType = pVar->GetType();
          if (!pType) continue;
          D3D11_SHADER_TYPE_DESC typeDesc;
          if (FAILED(pType->GetDesc(&typeDesc))) continue;

          GShaderValue v;
          v.name = varDesc.Name ? varDesc.Name : "";
          v.type = mapType(typeDesc);
          v.offset = varDesc.StartOffset;
          v.count = (typeDesc.Elements > 0) ? typeDesc.Elements : 1;

          m_values.push_back(v);
        }
      }

      // Reflect bound resources (textures, samplers)
      for (UINT r = 0; r < shaderDesc.BoundResources; ++r) {
        D3D11_SHADER_INPUT_BIND_DESC bindDesc;
        if (FAILED(pReflector->GetResourceBindingDesc(r, &bindDesc))) continue;

        GShaderValue v;
        v.name = bindDesc.Name ? bindDesc.Name : "";
        v.offset = 0;
        v.count = bindDesc.BindCount > 0 ? bindDesc.BindCount : 1;

        if (bindDesc.Type == D3D_SIT_TEXTURE) {
          // Map dimension to 2D or Cube when possible
          if (bindDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
            v.type = GSHADERPARAM::E::eTEXTURECUBE;
          else
            v.type = GSHADERPARAM::E::eTEXTURE2D; // default
          m_values.push_back(v);
        }
        else if (bindDesc.Type == D3D_SIT_SAMPLER) {
          v.type = GSHADERPARAM::E::eSAMPLERSTATE;
          m_values.push_back(v);
        }
        // Other resource types (structured buffers, UAVs) can be added later if needed
      }

      pReflector->Release();
    }
  }
}

GDX11PixelShader::~GDX11PixelShader()
{
  DX11_SAFE_RELEASE(m_pPixelShader);
  DX11_SAFE_RELEASE(m_pBlob->m_pBlob);
}

void* 
GDX11PixelShader::getShader() {
  return m_pPixelShader;
}

void 
GDX11PixelShader::set(void* pHandle) {
  m_pPixelShader = static_cast<ID3D11PixelShader*>(pHandle);
}

void* 
GDX11PixelShader::get() {
  return m_pPixelShader;
}

void
GDX11PixelShader::reflect() {
  if (m_pBlob && m_pBlob->m_pBlob) {
    ID3D11ShaderReflection* pReflector = nullptr;
    HRESULT hr = D3DReflect(m_pBlob->m_pBlob->GetBufferPointer(),
      m_pBlob->m_pBlob->GetBufferSize(),
      IID_ID3D11ShaderReflection,
      reinterpret_cast<void**>(&pReflector));
    if (SUCCEEDED(hr)) {
      D3D11_SHADER_DESC shaderDesc; pReflector->GetDesc(&shaderDesc);
      m_values.clear();

      auto mapType = [](const D3D11_SHADER_TYPE_DESC& tdesc) -> GSHADERPARAM::E {
        if (tdesc.Class == D3D_SVC_SCALAR) {
          if (tdesc.Type == D3D_SVT_FLOAT) return GSHADERPARAM::E::eFLOAT;
          if (tdesc.Type == D3D_SVT_INT)   return GSHADERPARAM::E::eINT;
          if (tdesc.Type == D3D_SVT_UINT)  return GSHADERPARAM::E::eUINT;
        } else if (tdesc.Class == D3D_SVC_VECTOR) {
          if (tdesc.Columns == 2) return GSHADERPARAM::E::eVECTOR2;
          if (tdesc.Columns == 3) return GSHADERPARAM::E::eVECTOR3;
          if (tdesc.Columns == 4) return GSHADERPARAM::E::eVECTOR4;
        } else if (tdesc.Class == D3D_SVC_MATRIX_ROWS || tdesc.Class == D3D_SVC_MATRIX_COLUMNS) {
          if ((tdesc.Rows == 4 && tdesc.Columns == 4) || (tdesc.Rows == 1 && tdesc.Columns == 16))
            return GSHADERPARAM::E::eMATRIX4;
          if ((tdesc.Rows == 3 && tdesc.Columns == 3) || (tdesc.Rows == 1 && tdesc.Columns == 9))
            return GSHADERPARAM::E::eMATRIX3;
        }
        return GSHADERPARAM::E::eUNKNOWN;
      };

      for (UINT cbIndex = 0; cbIndex < shaderDesc.ConstantBuffers; ++cbIndex) {
        ID3D11ShaderReflectionConstantBuffer* pCB = pReflector->GetConstantBufferByIndex(cbIndex);
        if (!pCB) continue;
        D3D11_SHADER_BUFFER_DESC cbDesc; if (FAILED(pCB->GetDesc(&cbDesc))) continue;
        for (UINT varIndex = 0; varIndex < cbDesc.Variables; ++varIndex) {
          ID3D11ShaderReflectionVariable* pVar = pCB->GetVariableByIndex(varIndex);
          if (!pVar) continue;
          D3D11_SHADER_VARIABLE_DESC varDesc; if (FAILED(pVar->GetDesc(&varDesc))) continue;
          ID3D11ShaderReflectionType* pType = pVar->GetType(); if (!pType) continue;
          D3D11_SHADER_TYPE_DESC typeDesc; if (FAILED(pType->GetDesc(&typeDesc))) continue;

          GShaderValue v; v.name = varDesc.Name ? varDesc.Name : "";
          v.type = mapType(typeDesc); v.offset = varDesc.StartOffset;
          v.count = (typeDesc.Elements > 0) ? typeDesc.Elements : 1;
          m_values.push_back(v);
        }
      }

      for (UINT r = 0; r < shaderDesc.BoundResources; ++r) {
        D3D11_SHADER_INPUT_BIND_DESC bindDesc; if (FAILED(pReflector->GetResourceBindingDesc(r, &bindDesc))) continue;
        GShaderValue v; v.name = bindDesc.Name ? bindDesc.Name : ""; v.offset = 0; v.count = bindDesc.BindCount > 0 ? bindDesc.BindCount : 1;
        if (bindDesc.Type == D3D_SIT_TEXTURE) {
          if (bindDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE) v.type = GSHADERPARAM::E::eTEXTURECUBE;
          else v.type = GSHADERPARAM::E::eTEXTURE2D;
          m_values.push_back(v);
        } else if (bindDesc.Type == D3D_SIT_SAMPLER) {
          v.type = GSHADERPARAM::E::eSAMPLERSTATE; m_values.push_back(v);
        }
      }

      pReflector->Release();
    }
  }
}

GDX11GeometryShader::~GDX11GeometryShader()
{
  DX11_SAFE_RELEASE(m_pGeometryShader);
  DX11_SAFE_RELEASE(m_pBlob->m_pBlob);
}

void*
GDX11GeometryShader::getShader() {
  return m_pGeometryShader;
}

void
GDX11GeometryShader::set(void* pHandle) {
  m_pGeometryShader = static_cast<ID3D11GeometryShader*>(pHandle);
}

void*
GDX11GeometryShader::get() {
  return m_pGeometryShader;
}

void
GDX11GeometryShader::reflect() {
  if (m_pBlob && m_pBlob->m_pBlob) {
    ID3D11ShaderReflection* pReflector = nullptr;
    HRESULT hr = D3DReflect(m_pBlob->m_pBlob->GetBufferPointer(),
      m_pBlob->m_pBlob->GetBufferSize(),
      IID_ID3D11ShaderReflection,
      reinterpret_cast<void**>(&pReflector));
    if (SUCCEEDED(hr)) {
      D3D11_SHADER_DESC shaderDesc; pReflector->GetDesc(&shaderDesc);
      m_values.clear();

      auto mapType = [](const D3D11_SHADER_TYPE_DESC& tdesc) -> GSHADERPARAM::E {
        if (tdesc.Class == D3D_SVC_SCALAR) {
          if (tdesc.Type == D3D_SVT_FLOAT) return GSHADERPARAM::E::eFLOAT;
          if (tdesc.Type == D3D_SVT_INT)   return GSHADERPARAM::E::eINT;
          if (tdesc.Type == D3D_SVT_UINT)  return GSHADERPARAM::E::eUINT;
        } else if (tdesc.Class == D3D_SVC_VECTOR) {
          if (tdesc.Columns == 2) return GSHADERPARAM::E::eVECTOR2;
          if (tdesc.Columns == 3) return GSHADERPARAM::E::eVECTOR3;
          if (tdesc.Columns == 4) return GSHADERPARAM::E::eVECTOR4;
        } else if (tdesc.Class == D3D_SVC_MATRIX_ROWS || tdesc.Class == D3D_SVC_MATRIX_COLUMNS) {
          if ((tdesc.Rows == 4 && tdesc.Columns == 4) || (tdesc.Rows == 1 && tdesc.Columns == 16))
            return GSHADERPARAM::E::eMATRIX4;
          if ((tdesc.Rows == 3 && tdesc.Columns == 3) || (tdesc.Rows == 1 && tdesc.Columns == 9))
            return GSHADERPARAM::E::eMATRIX3;
        }
        return GSHADERPARAM::E::eUNKNOWN;
      };

      for (UINT cbIndex = 0; cbIndex < shaderDesc.ConstantBuffers; ++cbIndex) {
        ID3D11ShaderReflectionConstantBuffer* pCB = pReflector->GetConstantBufferByIndex(cbIndex);
        if (!pCB) continue;
        D3D11_SHADER_BUFFER_DESC cbDesc; if (FAILED(pCB->GetDesc(&cbDesc))) continue;
        for (UINT varIndex = 0; varIndex < cbDesc.Variables; ++varIndex) {
          ID3D11ShaderReflectionVariable* pVar = pCB->GetVariableByIndex(varIndex);
          if (!pVar) continue;
          D3D11_SHADER_VARIABLE_DESC varDesc; if (FAILED(pVar->GetDesc(&varDesc))) continue;
          ID3D11ShaderReflectionType* pType = pVar->GetType(); if (!pType) continue;
          D3D11_SHADER_TYPE_DESC typeDesc; if (FAILED(pType->GetDesc(&typeDesc))) continue;
          GShaderValue v; v.name = varDesc.Name ? varDesc.Name : "";
          v.type = mapType(typeDesc); v.offset = varDesc.StartOffset;
          v.count = (typeDesc.Elements > 0) ? typeDesc.Elements : 1;
          m_values.push_back(v);
        }
      }

      for (UINT r = 0; r < shaderDesc.BoundResources; ++r) {
        D3D11_SHADER_INPUT_BIND_DESC bindDesc; if (FAILED(pReflector->GetResourceBindingDesc(r, &bindDesc))) continue;
        GShaderValue v; v.name = bindDesc.Name ? bindDesc.Name : ""; v.offset = 0; v.count = bindDesc.BindCount > 0 ? bindDesc.BindCount : 1;
        if (bindDesc.Type == D3D_SIT_TEXTURE) {
          if (bindDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE) v.type = GSHADERPARAM::E::eTEXTURECUBE;
          else v.type = GSHADERPARAM::E::eTEXTURE2D; m_values.push_back(v);
        } else if (bindDesc.Type == D3D_SIT_SAMPLER) {
          v.type = GSHADERPARAM::E::eSAMPLERSTATE; m_values.push_back(v);
        }
      }
      pReflector->Release();
    }
  }
}

GDX11ComputeShader::~GDX11ComputeShader()
{
  DX11_SAFE_RELEASE(m_pComputeShader);
  DX11_SAFE_RELEASE(m_pBlob->m_pBlob);
}

void*
GDX11ComputeShader::getShader() {
  return m_pComputeShader;
}

void
GDX11ComputeShader::set(void* pHandle) {
  m_pComputeShader = static_cast<ID3D11ComputeShader*>(pHandle);
}

void*
GDX11ComputeShader::get() {
  return m_pComputeShader;
}

void
GDX11ComputeShader::reflect() {
  if (m_pBlob && m_pBlob->m_pBlob) {
    ID3D11ShaderReflection* pReflector = nullptr;
    HRESULT hr = D3DReflect(m_pBlob->m_pBlob->GetBufferPointer(),
      m_pBlob->m_pBlob->GetBufferSize(),
      IID_ID3D11ShaderReflection,
      reinterpret_cast<void**>(&pReflector));
    if (SUCCEEDED(hr)) {
      D3D11_SHADER_DESC shaderDesc; pReflector->GetDesc(&shaderDesc);
      m_values.clear();

      auto mapType = [](const D3D11_SHADER_TYPE_DESC& tdesc) -> GSHADERPARAM::E {
        if (tdesc.Class == D3D_SVC_SCALAR) {
          if (tdesc.Type == D3D_SVT_FLOAT) return GSHADERPARAM::E::eFLOAT;
          if (tdesc.Type == D3D_SVT_INT)   return GSHADERPARAM::E::eINT;
          if (tdesc.Type == D3D_SVT_UINT)  return GSHADERPARAM::E::eUINT;
        } else if (tdesc.Class == D3D_SVC_VECTOR) {
          if (tdesc.Columns == 2) return GSHADERPARAM::E::eVECTOR2;
          if (tdesc.Columns == 3) return GSHADERPARAM::E::eVECTOR3;
          if (tdesc.Columns == 4) return GSHADERPARAM::E::eVECTOR4;
        } else if (tdesc.Class == D3D_SVC_MATRIX_ROWS || tdesc.Class == D3D_SVC_MATRIX_COLUMNS) {
          if ((tdesc.Rows == 4 && tdesc.Columns == 4) || (tdesc.Rows == 1 && tdesc.Columns == 16))
            return GSHADERPARAM::E::eMATRIX4;
          if ((tdesc.Rows == 3 && tdesc.Columns == 3) || (tdesc.Rows == 1 && tdesc.Columns == 9))
            return GSHADERPARAM::E::eMATRIX3;
        }
        return GSHADERPARAM::E::eUNKNOWN;
      };

      for (UINT cbIndex = 0; cbIndex < shaderDesc.ConstantBuffers; ++cbIndex) {
        ID3D11ShaderReflectionConstantBuffer* pCB = pReflector->GetConstantBufferByIndex(cbIndex);
        if (!pCB) continue;
        D3D11_SHADER_BUFFER_DESC cbDesc; if (FAILED(pCB->GetDesc(&cbDesc))) continue;
        for (UINT varIndex = 0; varIndex < cbDesc.Variables; ++varIndex) {
          ID3D11ShaderReflectionVariable* pVar = pCB->GetVariableByIndex(varIndex);
          if (!pVar) continue;
          D3D11_SHADER_VARIABLE_DESC varDesc; if (FAILED(pVar->GetDesc(&varDesc))) continue;
          ID3D11ShaderReflectionType* pType = pVar->GetType(); if (!pType) continue;
          D3D11_SHADER_TYPE_DESC typeDesc; if (FAILED(pType->GetDesc(&typeDesc))) continue;
          GShaderValue v; v.name = varDesc.Name ? varDesc.Name : "";
          v.type = mapType(typeDesc); v.offset = varDesc.StartOffset;
          v.count = (typeDesc.Elements > 0) ? typeDesc.Elements : 1;
          m_values.push_back(v);
        }
      }

      for (UINT r = 0; r < shaderDesc.BoundResources; ++r) {
        D3D11_SHADER_INPUT_BIND_DESC bindDesc; if (FAILED(pReflector->GetResourceBindingDesc(r, &bindDesc))) continue;
        GShaderValue v; v.name = bindDesc.Name ? bindDesc.Name : ""; v.offset = 0; v.count = bindDesc.BindCount > 0 ? bindDesc.BindCount : 1;
        if (bindDesc.Type == D3D_SIT_TEXTURE) {
          if (bindDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE) v.type = GSHADERPARAM::E::eTEXTURECUBE;
          else v.type = GSHADERPARAM::E::eTEXTURE2D; m_values.push_back(v);
        } else if (bindDesc.Type == D3D_SIT_SAMPLER) {
          v.type = GSHADERPARAM::E::eSAMPLERSTATE; m_values.push_back(v);
        }
      }
      pReflector->Release();
    }
  }
}

}
