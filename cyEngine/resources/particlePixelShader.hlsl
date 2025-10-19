//type:pixel
#define PI 3.1415926535f
#define TAU 6.283185307f

Texture2D txColor : register(t0);

SamplerState samPoint   : register(s0);
SamplerState samLinear  : register(s1);
SamplerState samAniso   : register(s2);

struct PixelInput
{
  float4 position   : SV_POSITION;
  float2 texCoord   : TEXCOORD1;
  float3 posW       : TEXCOORD3;
};

cbuffer ShaderConstants : register(b0)
{
  float time;
}

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

float3
hsv2rgb_smooth(in float3 c)
{
  float3 rgb = clamp(abs(fmod(c.x * 6.0f + float3(0.0f, 4.0f, 2.0f), 6.0f) - 3.0f) - 1.0f, 0.0f, 1.0f);

  rgb = rgb * rgb * (3.0f - 2.0f * rgb); // cubic smoothing

  return c.z * lerp(float3(1.0f, 1.0f, 1.0f), rgb, c.y);
}

struct PixelOutput
{
  float4 color : SV_TARGET0;
};

PixelOutput
pixel_main(PixelInput Input)
{
  PixelOutput Output = (PixelOutput)0;
    
  float4 albedo = txColor.Sample(samPoint, Input.texCoord);
  clip(albedo.a <= 0.5f ? -1.0f : 1.0f); // discard if alpha is too low
  Output.color = float4(1.0f, 1.0f, 1.0f, albedo.a);
    
  return Output;
}