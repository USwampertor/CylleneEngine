//type:pixel
#define PI 3.1415926535f
#define TAU 6.283185307f

Texture2D txColor : register(t0);

SamplerState samPoint : register(s0);
SamplerState samLinear : register(s1);
SamplerState samAniso : register(s2);

struct PixelInput
{
  float4 position : SV_POSITION;
  float4 color : TEXCOORD0;
  float3 normal : TEXCOORD2;
  float3 tangent : TEXCOORD4;
  float3 binormal : BINORMAL0;
  float2 texCoord : TEXCOORD1;
  float3 posW : TEXCOORD3;
};

cbuffer ShaderConstants : register(b0)
{
  float time;
  float align1;
  float align2;
  float align3;
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
  float4 position : SV_TARGET1;
};

PixelOutput
pixel_main(PixelInput Input)
{
  PixelOutput Output = (PixelOutput) 0;
    
  float3 position = Input.posW;
  float3 albedo = txColor.Sample(samLinear, Input.texCoord).rgb;
    
  float3 acc = float3(0.1f, 0.1f, 0.1f);

  float lightoffset = 8.0f;
  float lightRadius = 4.0f;
  float numLights = 10;
    
  for (float p = 0.0f; p < 1.0f; p += 1.0f / numLights)
  {
    float d = p * TAU;
    float3 lightPos = float3(sin(d + time * 1.0) * lightoffset,
                             3.0f + cos(time),
                             cos(d + time * 3.0) * lightoffset);
    
    float lightAtt = 1.0f - min(1.0f, length(lightPos - position) / lightRadius);
    float3 lightColor = hsv2rgb_smooth(float3(p, 1.0f, 1.0f));
    acc += lightColor * lightAtt;
  }
    
  Output.color = float4(albedo * acc, 1.0f);
  Output.position = float4(position, 1.0f);
    
  return Output;
}