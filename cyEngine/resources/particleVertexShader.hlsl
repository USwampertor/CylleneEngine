//type:vertex
#define PI 3.1415926535f
#define TAU 6.283185307f

Texture2D txColor : register(t0);

SamplerState samPoint : register(s0);
SamplerState samLinear : register(s1);
SamplerState samAniso : register(s2);

struct VertexInput
{
  float3    position    : POSITION;
  float3    normal      : NORMAL0;
  float3    tangent     : TANGENT0;
  float3    binormal    : BINORMAL0;
  float4    color       : COLOR0;
  float2    texCoord    : TEXCOORD0;
  int4      bones       : BONES;
  float4    weights     : WEIGHTS;
  int4      metadata    : METADATA;
};

struct PixelInput
{
  float4 position   : SV_POSITION;
  float2 texCoord   : TEXCOORD1;
  float3 posW       : TEXCOORD3;
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

float hash11(uint n) {
    // integer hash copied from Hugo Elias
    n = (n << 13U) ^ n;
    n = n * (n * n * 15731U + 789221U) + 1376312589U;
    return float(n & uint(0x7fffffffU)) / float(0x7fffffff);
}

PixelInput vertex_main(VertexInput Input, uint instanceID : SV_InstanceID)
{
  PixelInput Output = (PixelInput) 0;
    
  Output.position = CameraUp * Input.position.y +
                    CameraRight * Input.position.x;
  Output.position.w = 1.0f;
    
  float layer = 1.0f + floor(instanceID / 10.0f) * 0.1f;
  float invDir = fmod(floor(instanceID / 10.0f), 2.0f) == 0.0f ? 1.0f : -1.0f;
  float separation = 3.0f + layer * 3.0f;
  float ox = sin(invDir * 0.6f * layer * time + (TAU / 10.0f) * instanceID);
  float oz = cos(invDir * 0.6f * layer * time + (TAU / 10.0f) * instanceID);
  float oy = floor(instanceID / 10.0f) * 1.5f;
    
  Output.position.x += ox * separation;
  Output.position.y += 1.0f + oy;
  Output.position.z += oz * separation;
    
  //Output.position = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), World);
  Output.position = mul(Output.position, World);
  Output.posW = Output.position.xyz;
    
  Output.position = mul(Output.position, View);
  Output.position = mul(Output.position, Projection);
    
  Output.texCoord = Input.texCoord;
    
  float timeScaled = time * 20.0f;
  float offset = fmod(floor((hash11(instanceID) * 12.0f) + timeScaled), 12.0f) * (1.0f / 12.0f);
  Output.texCoord.x = (Output.texCoord.x * (1.0f / 12.0f)) + offset;
  
  return Output;
}