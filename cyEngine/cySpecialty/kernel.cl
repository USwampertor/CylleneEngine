__kernel void vectorAdd(__global const float* A,
  __global const float* B,
  __global float* C,
  const unsigned int numElements) {
  int i = get_global_id(0);

  C[i] = A[i] + B[i];
}

__kernel void matrixAdd(__global const float* A,
  __global const float* B,
  __global float* C,
  const unsigned int width,
  const unsigned int height) {
  int row = get_global_id(0);
  int col = get_global_id(1);

  if (row < height && col < width) {
    int index = row * width + col;
    C[index] = A[index] + B[index];
  }
}

__attribute__((reqd_work_group_size(16, 16, 1)))
__kernel void optimizedMatrixAdd(__global const float* A,
  __global const float* B,
  __global float* C,
  const unsigned int width,
  const unsigned int height) {
  int row = get_global_id(0);
  int col = get_global_id(1);

  if (row < height && col < width) {
    int index = row * width + col;
    C[index] = A[index] + B[index];
  }
}

// __kernel void clear()
// {
//   int i = get_global_id(0);
//   V[i] = 0;
//   barrier(CLK_GLOBAL_MEM_FENCE);
//   V[i] += 1;
// }


__kernel void histogram_kernel_2D(__global const uchar* colorChannel,
                                  const uint width,
                                  const uint height,
                                  __global uint* numLevels) {

  __local uint lnumLevels[256];

  // TODO: This kernel assumes that width*height is divisible by 4
  const size_t N = (size_t)width * (size_t)height;
  const size_t N4 = N / 4;

  // Map image to use 4 bytes at the same time
  __global const uchar4* colorChannel4 = (__global const uchar4*)colorChannel;

  const uint lidX = get_local_id(0);
  const uint lidY = get_local_id(1);
  const uint lsizeX = get_local_size(0);
  const uint lsizeY = get_local_size(1);

  const uint gidX = get_group_id(0);
  const uint gidY = get_group_id(1);

  const uint tileW = lsizeX;
  const uint tileH = lsizeY;

  const uint x0 = gidX * tileW;
  const uint y0 = gidY * tileH;


  const uint lID = lidY * lsizeX + lidX;
  const uint lSize = lsizeX * lsizeY;
  __local uint lNumLevels[256 * 4];

  // Clear local histogram
  // for (uint i = lidY * lsizeX + lidX; i < 256; i += lsizeX * lsizeY) {
  for (uint i = lID; i < 256 * 4; i += lSize) {
    lnumLevels[i] = 0;
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  // Assign replica to each thread
  const uint replica = lID & 3u;

  const size_t gIDLineal = (size_t)get_global_id(1) * (size_t)(get_global_size(0)) + (size_t)get_global_id(0);
  const size_t gSizeLineal = (size_t)(get_global_size(0)) * (size_t)(get_global_size(1));

  // Local tile accumulation
  // for (uint y = y0 + lidY; y < min(y0 + tileH, height); y += lsizeY) {
  // uint row = y * width;
  // for (uint x = x0 + lidX; x < min(x0 + tileW, width); x += lsizeX) {
  //   uchar container = colorChannel[row + x];
  //   atomic_inc(&lnumLevels[container]);
  // }
  for (uint i4 = gIDLineal; i4 < N4; i4 += gSizeLineal) {
    uchar4 pixel = colorChannel4[i4];
    uchar cont0 = (uint)pixel.x;
    uchar cont1 = (uint)pixel.y;
    uchar cont2 = (uint)pixel.z;
    uchar cont3 = (uint)pixel.w;
    atomic_inc(&lnumLevels[cont0 * 4u + replica]);
    atomic_inc(&lnumLevels[cont1 * 4u + replica]);
    atomic_inc(&lnumLevels[cont2 * 4u + replica]);
    atomic_inc(&lnumLevels[cont3 * 4u + replica]);
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  // Merge local histograms into global histogram
  // for (uint i = lidY * lsizeX + lidX; i < 256; i += lsizeX * lsizeY) {
  //   uint v = lnumLevels[i];
  //   if (v != 0) {
  //     atomic_add(&numLevels[i], v);
  //   }
  // }

  for (uint i = lID; i < 256u; i += lSize) {
    uint v0 = lnumLevels[i * 4u + 0u];
    uint v1 = lnumLevels[i * 4u + 1u];
    uint v2 = lnumLevels[i * 4u + 2u];
    uint v3 = lnumLevels[i * 4u + 3u];
    uint v = v0 + v1 + v2 + v3;
    if (v != 0) {
      atomic_add(&numLevels[i], v);
    }
  }

  // uchar container = colorChannel[i];
  // atomic_inc(&numLevels[container]);
}

__kernel void histogram_kernel(__global const uchar* colorChannel,
  const uint numPixels,
  __global uint* numLevels) {

  __local uint lnumLevels[256];

  const uint gi = get_global_id(0);
  const uint gsize = get_global_size(0);
  const uint lid = get_local_id(0);
  const uint lsize = get_local_size(0);

  // Clear local histogram
  for (uint i = lid; i < 256; i += lsize) {
    lnumLevels[i] = 0;
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  // Local histogram computation
  for (uint i = gi; i < numPixels; i += gsize) {
    uchar container = colorChannel[i];
    atomic_inc(&lnumLevels[container]);
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  // Merge local histograms into global histogram
  for (uint i = lid; i < 256; i += lsize) {
    uint v = lnumLevels[i];
    if (v != 0) {
      atomic_add(&numLevels[i], v);
    }
  }

  // uchar container = colorChannel[i];
  // atomic_inc(&numLevels[container]);
}

typedef struct Agent
{
  float2 position;
  float2 velocity;
}Agent;

float2 limit_length(float2 vec, float maxLength) {
  float len2 = dot(vec, vec);
  if (len2 > maxLength * maxLength) {
    float invLen = native_rsqrt(len2);
    vec *= maxLength * invLen;
  }
  return vec;
}

__kernel void steering_seek(__global Agent* agents, 
                            const float2 target,
                            const float maxSpeed,
                            const float maxForce,
                            const float deltaTime) {
  uint gid = get_global_id(0);
  uint numAgents = get_global_size(0);
  if (gid >= numAgents) { return; }

  Agent agent = agents[gid];

  float2 desired = target - agent.position;
  float dist = length(desired);
  
  desired /= dist; // Normalize
  float speed = maxSpeed;
  float2 steer = desired - agent.velocity;
  steer = limit_length(steer, maxForce);

  // Movement integration
  agent.velocity += steer * deltaTime;
  agent.velocity = limit_length(agent.velocity, maxSpeed);
  agent.position += agent.velocity * deltaTime;
  agents[gid] = agent;

}