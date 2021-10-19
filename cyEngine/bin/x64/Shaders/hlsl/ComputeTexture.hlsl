#define BLOCK_SIZE uint3(40, 23, 1)
#define THREADS_SIZE uint3(32, 32, 1)

RWTexture2D<unorm float4> backbuffer : register(u0);

/**
* uint3
* SV_GroupThreadID:
*   Defines the thread offset within the group, per dimension of the group.
* 
* uint3
* SV_GroupID:
*   Defines the group offset within a Dispatch call, per dimension of the dispatch call.
* 
* uint3
* SV_DispatchThreadID:
*   Defines the global thread offset within the Dispatch call, per dimension of the group.
* 
* uint
* SV_GroupIndex:
*   Provides a flattened index for a given thread within a given group.
*/

[numthreads(32, 32, 1)]
void
main(uint3 local_thread_ID  : SV_GroupThreadID,
     uint3 group_ID         : SV_GroupID,
     uint3 global_thread_ID : SV_DispatchThreadID,
     uint  thread_index     : SV_GroupIndex) {
  if (global_thread_ID.x > 1280 || global_thread_ID.y > 720) return;
  float2 uv = global_thread_ID.xy / float2(1280.0f, 720.0f);
  
  float3 color = backbuffer[global_thread_ID.xy].xyz;

  float vignette = distance(uv, float2(0.5f, 0.5f));
  //vignette /= 0.5f;
  vignette /= 0.70711f;

  //backbuffer[global_thread_ID.xy] = float4(uv, 0.0f, 1.0f);
  //backbuffer[global_thread_ID.xy] = float4(color, 1.0f);
  //backbuffer[global_thread_ID.xy] = float4(1.0f.xxx - color, 1.0f);
  backbuffer[global_thread_ID.xy] = float4(lerp(color, 0.0f.xxx, vignette), 1.0f);
}