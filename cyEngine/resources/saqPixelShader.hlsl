Texture2D colorTexture          : register(t0);
Texture2D positionTexture       : register(t1);
Texture2D<float> depthTexture   : register(t2);

SamplerState samPoint   : register(s0);
SamplerState samLinear  : register(s1);
SamplerState samAniso   : register(s2);

struct PixelInput
{
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD0;
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

float4
pixel_main(PixelInput Input) : SV_TARGET {
    float2 uv = Input.texCoord;
    
    float3 color = colorTexture.Sample(samLinear, uv).rgb;
    float3 position = positionTexture.Sample(samLinear, uv).xyz;
    
    float4 shadowPosition   = float4(position, 1.0f);
    shadowPosition          = mul(shadowPosition, ShadowView);
    shadowPosition          = mul(shadowPosition, ShadowProjection);
    
    float4 shadowCoord = shadowPosition;
    shadowCoord.xyz /= shadowCoord.w;
    shadowCoord.xy = (shadowCoord.xy * 0.5f) + 0.5f;
    shadowCoord.xy = saturate(shadowCoord.xy);
    
    shadowCoord.y = 1.0f - shadowCoord.y;
    float currentDepth = shadowCoord.z;
  
    float shadowDepth = depthTexture.Sample(samPoint, shadowCoord.xy).r;
    
    float isDepth = step((currentDepth - 0.00001f), shadowDepth);
    //float isDepth = step((currentDepth - 0.005f), shadowDepth);
    
    return float4(color * isDepth, 1.0f);
}