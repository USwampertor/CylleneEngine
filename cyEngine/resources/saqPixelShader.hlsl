//type:pixel
// GBuffer inputs
Texture2D colorTexture          : register(t0);
Texture2D positionTexture       : register(t1);

// Shadow maps array (bind starting at t2)
#ifndef MAX_SHADOW_MAPS
#define MAX_SHADOW_MAPS 8
#endif
Texture2D<float> depthTextures[MAX_SHADOW_MAPS] : register(t2);

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
    // float align1;
    // float align2;
    // float align3;
}

cbuffer ShadowConstantBuffer : register(b3)
{
    float4x4 ShadowView;
    float4x4 ShadowProjection;
    uint ShadowCount; float3 _pad_shadow;
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
  
    // Accumulate visibility across all shadow maps
    float isDepth = 1.0f;
    // Unroll so resource array indices are literals at compile-time
    [unroll]
    for (uint i = 0; i < MAX_SHADOW_MAPS; ++i) {
        if (i < ShadowCount) {
            float shadowDepth = depthTextures[i].Sample(samPoint, shadowCoord.xy).r;
            float vis = step((currentDepth - 0.00001f), shadowDepth);
            isDepth = min(isDepth, vis);
        }
    }
    //float isDepth = step((currentDepth - 0.005f), shadowDepth);
    
    [branch]
    if (position.x == 0 &&
        position.y == 0 &&
        position.z == 0) {
        isDepth = 1.0f;
    }
    
    return float4(color * isDepth, 1.0f);
}
