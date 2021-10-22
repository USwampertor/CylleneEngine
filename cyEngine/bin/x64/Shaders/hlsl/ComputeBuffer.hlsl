#define BLOCK_SIZE uint3(4, 4, 1)
#define THREADS_SIZE uint3(8, 8, 1)

RWBuffer<float> heights : register(u0);

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
  uint3 totalThreadsSize = BLOCK_SIZE * THREADS_SIZE;
  uint index = (global_thread_ID.y * totalThreadsSize.x) + global_thread_ID.x;
  heights[index] = index;
}