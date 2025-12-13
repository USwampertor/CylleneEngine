struct PSInput
{
  float4 position : SV_POSITION;
  float4 color : COLOR;
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR)
{
  PSInput output;
  output.position = position;
  output.color = color;
  return output;
}

float PSMain(PSInput input) : SV_TARGET
{
  return input.color;
}
