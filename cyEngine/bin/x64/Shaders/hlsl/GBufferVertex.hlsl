Buffer<float> heights : register(t0);

cbuffer ConstantBuffer : register(b0) {
  float4x4 World;
  float4x4 View;
  float4x4 Projection;
};

struct VS_INPUT {
  float3 Position   : POSITION;
  float2 UV         : TEXCOORD0;
  float3 Normal     : NORMAL;
  //uint   instanceID : SV_InstanceID;
};

struct VS_OUTPUT {
  float4 Position  : SV_POSITION;
  float3 World     : TEXCOORD0;
  float3 Normal    : TEXCOORD1;
  float2 UV        : TEXCOORD2;
};

VS_OUTPUT
main(VS_INPUT input) {
  VS_OUTPUT vsOut;
  
  float3 position = input.Position;
  //float3 position = input.Position * 0.25f;
  position = mul(World, float4(position, 1.0f)).xyz;

  //position.x += (input.instanceID % 32) * 1.5f;
  //position.z += floor(input.instanceID / 32.0f) * 1.5f;
  //position.y += heights[input.instanceID] * 0.05f;
  
  float4 vertex = mul(Projection, mul(View, float4(position, 1.0f)));
  
  vsOut.Position = vertex;
  vsOut.World    = position;
  vsOut.Normal   = input.Normal;
  vsOut.UV       = input.UV;

  return vsOut;
}