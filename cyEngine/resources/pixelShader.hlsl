Texture2D txColor : register(t0);

SamplerState samPoint : register(s0);
SamplerState samLinear : register(s1);
SamplerState samAniso : register(s2);

struct PixelInput
{
  float4 position : SV_POSITION;
  float4 color : TEXCOORD0;
  float2 texCoord : TEXCOORD1;
};

float4 pixel_main(PixelInput Input) : SV_TARGET {
  
  float4 color = txColor.Sample(samLinear, Input.texCoord);
  
  return color;
  
  // return float4(Input.texCoord, 0.0f, 1.0f);
}