//type:compute

RWTexture2D<float4> outTex : register(u0);

#define NUMTHREADS_X 32
#define NUMTHREADS_Y 32
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
compute_main(uint3 groupThreadID	: SV_GroupThreadID,
             uint3 groupID				: SV_GroupID,
             uint3 dispatchID			: SV_DispatchThreadID,
             uint  groupIndex			: SV_GroupIndex) {
  uint2 pixel = dispatchID.xy;

  if (pixel.y >= 720) return;
  
  outTex[pixel] = float4(float3(1.0f, 1.0f, 1.0f) - outTex[pixel].rgb, 1.0f);
}

