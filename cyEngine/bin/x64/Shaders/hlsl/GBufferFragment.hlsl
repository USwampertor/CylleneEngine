Texture2D ourTexture  : register(t0);

SamplerState SS;

cbuffer ConstantBuffer {
  float4x4 View;
  float4x4 Projection;
};

struct PS_INPUT {
  float4 Position  : SV_POSITION;
  float2 UV        : TEXCOORD0;
  float3 Normal    : TEXCOORD1;
};

struct PS_OUTPUT {
  float4 Fragment  : SV_TARGET0;
};

PS_OUTPUT
main(PS_INPUT input) {
  PS_OUTPUT psOut;
  
  //psOut.Fragment = float4(ourTexture.Sample(SS, input.UV).rgb, 1.0f);
  //psOut.Fragment = float4(ourTexture.Sample(SS, input.UV).rgb, 1.0f);

  return psOut;
}