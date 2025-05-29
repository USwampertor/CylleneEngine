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
};

cbuffer PerObjectConstantBuffer : register(b1)
{
    float4x4 World;
}

cbuffer PerPassConstantBuffer : register(b2)
{
  float4   CameraForward;
  float4   CameraRight;
  float4   CameraUp;
  float4x4 View;
  float4x4 Projection;
}

PixelInput vertex_main(VertexInput Input, uint vertex_index : SV_VertexID) {
  PixelInput Output = (PixelInput)0;
    
  Output.position = float4(Input.position, 1);
    
  Output.position = mul(Output.position, World);
  Output.position = mul(Output.position, View);
  Output.position = mul(Output.position, Projection);
  
  return Output;
}