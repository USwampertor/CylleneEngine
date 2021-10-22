Texture2D ourTexture  : register(t0);

SamplerState SS;

cbuffer ConstantBuffer : register(b0) {
  float4x4 View;
  float4x4 Projection;
};

struct PS_INPUT {
  float4 Position  : SV_POSITION;
  float3 World     : TEXCOORD0;
  float3 Normal    : TEXCOORD1;
  float2 UV        : TEXCOORD2;
};

struct PS_OUTPUT {
  float4 Fragment  : SV_TARGET0;
};

PS_OUTPUT
main(PS_INPUT input) {
  PS_OUTPUT psOut;
  float2 uv = input.UV;
  
  float4 color = ourTexture.Sample(SS, uv).bgra;
  if (color.a < 0.5f) discard;
  
  psOut.Fragment = float4(color.rgb, input.Position.z);

  return psOut;
}