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
  float4 AlbedoDepth  : SV_TARGET0;
  float4 Normal       : SV_TARGET1;
};

float
SDF_2D_Circle(float2 p, float r) {
  return length(p) - r;
}

float
SDF_2D_Square(float2 p, float2 b) {
  float2 d = abs(p) - b;
  return length(max(d, 0.0f)) + min(max(d.x, d.y), 0.0f);
}

PS_OUTPUT
main(PS_INPUT input) {
  PS_OUTPUT psOut;
  float2 uv = input.UV;
  
  // Texture
  float4 color = ourTexture.Sample(SS, uv).rgba;
  if (color.a < 0.5f) discard;

  psOut.AlbedoDepth = float4(color.rgb, input.Position.z);
  psOut.Normal      = float4(normalize(input.Normal), 1.0f);
  
  /*
  // SDF
  float2 p = (uv * 2.0f) - 1.0f;
  float cicleSDF = SDF_2D_Circle(p, 1.0f);
  if (cicleSDF > 0.0f) discard;
  
  float3 color = 0.0f;

  // Circle gradient
  color = lerp(0.5f.xxx,
               0.4f.xxx,
               length(p));

  // Square line
  float squareSDF = SDF_2D_Square(p - float2(0.0f, 0.5f), float2(0.05f, 0.5f));
  color = lerp(color,
               float3(1.0f, 1.0f, 1.0f),
               step(squareSDF, 0.0f));

  // Circle border
  color = lerp(float3(1.0f, 0.5f, 0.26f),
               color,
               step(cicleSDF, -0.15f));
  
  psOut.AlbedoDepth = float4(color, input.Position.z);
  psOut.Normal      = float4(input.Normal, 1.0f);
  */

  return psOut;
}