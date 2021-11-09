#define BLOCK_SIZE uint3(8, 8, 1)
#define THREADS_SIZE uint3(32, 32, 1)

#define PI 3.1415926535f
#define TAU PI * 2.0f

struct agent {
  uint  position;
  uint  velocity;
};

cbuffer ConstantBuffer : register(b0) {
  float time;
  float deltaTime;
};

RWStructuredBuffer<agent> agents : register(u0);

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

float
getLower16(uint value) {
  return f16tof32(value);
}

float
getUpper16(uint value) {
  value = value >> 16;
  return f16tof32(value);
}

uint
pack_float2(float lowerValue, float upperValue) {
  uint lowerPart = f32tof16(lowerValue);
  uint upperPart = f32tof16(upperValue);

  upperPart = upperPart << 16;

  lowerPart &= 0x0000FFFF;
  upperPart &= 0xFFFF0000;

  return lowerPart | upperPart;
}


uint
pack_float2(float2 values) {
  return pack_float2(values.x, values.y);
}

float2
unpack_float2(uint value) {
  float2 result;

  result.x = getLower16(value);
  result.y = getUpper16(value);

  return result;
}

void
unpack_float2(uint value, inout float2 result) {
  result = unpack_float2(value);
}

float
hash(float p, inout int counter) {
  return frac(sin(dot(p, float2(12.9898, 78.233))) * (counter++ + 43758.5453));
}

float
nrand(float2 n) {
	return frac(sin(dot(n.xy, float2(12.9898, 78.233))) * 43758.5453);
}

float
n1rand(float2 n, float seed) {
	float t = frac(seed);
	float nrnd0 = nrand(n + 0.07 * t);
	return nrnd0;
}

float2
RotateAround(float2 coords, float2 center, float angle) {
  float sr = sin(angle);
  float cr = cos(angle);

  float2 newcoords = coords - center;
  newcoords.x = (cr * coords.x) - (sr * coords.y);
  newcoords.y = (sr * coords.x) + (cr * coords.y);

  return newcoords + center;
}


[numthreads(32, 32, 1)]
void
main(uint3 local_thread_ID  : SV_GroupThreadID,
     uint3 group_ID         : SV_GroupID,
     uint3 global_thread_ID : SV_DispatchThreadID,
     uint  thread_index     : SV_GroupIndex) {
  uint3 totalThreadsSize = BLOCK_SIZE * THREADS_SIZE;
  uint index = (global_thread_ID.y * totalThreadsSize.x) + global_thread_ID.x;
  
  float dt = max(deltaTime, 0.01f);

  float2 position = unpack_float2(agents[index].position);
  float2 velocity = unpack_float2(agents[index].velocity);

  float maxForce = 20.0f;
  float maxSpeed = 50.0f;
  
  int randomcounter = 0;
  
  float angle = hash(time + (index / 65000.0f), randomcounter) * TAU;

  float2 frameVelocity = RotateAround(float2(0.0f, 1.0f), 0.0f, angle);
  frameVelocity = normalize(frameVelocity);
  frameVelocity *= maxSpeed;
  
	float2 steer = frameVelocity - velocity;
	steer /= maxSpeed;
	steer *= maxForce;
  
  velocity += steer * dt;
  velocity = normalize(velocity) * clamp(length(velocity), 0, maxSpeed);

  position += velocity * dt;
  
  agents[index].position = pack_float2(position);
  agents[index].velocity = pack_float2(velocity);
}