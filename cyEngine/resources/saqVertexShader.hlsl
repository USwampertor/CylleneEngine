//type:vertex
struct VertexInput
{
  float3    position   : POSITION;
  float3    normal     : NORMAL0;
  float3    tangent    : TANGENT0;
  float3    binormal   : BINORMAL0;
  float4    color      : COLOR0;
  float2    texCoord   : TEXCOORD0;
  int4      bones      : BONES;
  float4    weights    : WEIGHTS;
  int4      metadata   : METADATA;
};

struct PixelInput
{
  float4 position   : SV_POSITION;
  float2 texCoord   : TEXCOORD0;
};

cbuffer ShaderConstants : register(b0)
{
    float time;
    float align1;
    float align2;
    float align3;
}

cbuffer ShadowConstantBuffer : register(b3)
{
    float4x4 ShadowView;
    float4x4 ShadowProjection;
}

PixelInput vertex_main(VertexInput Input, uint vertex_index : SV_VertexID) {
  PixelInput Output = (PixelInput)0;
    
  Output.position = float4(Input.position, 1);
  Output.texCoord = Input.texCoord;
  
  return Output;
}