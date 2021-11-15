#define PI 3.1415926535f
#define TAU PI * 2.0f

struct agent {
  uint  position;
  uint  velocity;
};

StructuredBuffer<agent> agents : register(t0);

cbuffer ConstantBuffer : register(b0) {
  float4x4 World;
  float4x4 View;
  float4x4 Projection;
};

struct VS_INPUT {
  float3 Position   : POSITION;
  float2 UV         : TEXCOORD0;
  float3 Normal     : NORMAL;
  uint   instanceID : SV_InstanceID;
};

struct VS_OUTPUT {
  float4 Position  : SV_POSITION;
  float3 World     : TEXCOORD0;
  float3 Normal    : TEXCOORD1;
  float2 UV        : TEXCOORD2;
};
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
  uint lowerPart = f32tof16(values.x);
  uint upperPart = f32tof16(values.y);

  upperPart = upperPart << 16;

  lowerPart &= 0x0000FFFF;
  upperPart &= 0xFFFF0000;

  return lowerPart | upperPart;
}

void
unpack_float2(uint value, inout float2 result) {
  result.x = getLower16(value);
  result.y = getUpper16(value);
}

float2
unpack_float2(uint value) {
  float2 result;

  result.x = getLower16(value);
  result.y = getUpper16(value);

  return result;
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

VS_OUTPUT
main(VS_INPUT input) {
  VS_OUTPUT vsOut;
  
  float3 position = input.Position;
  float3 normal = input.Normal;
  
  // This is for the agent looking direction
  /*
  float2 velocity = unpack_float2(agents[input.instanceID].velocity);
  float angle = atan2(velocity.y, velocity.x);
  position.xy = RotateAround(position.xy, 0.0f, angle + TAU * 0.25f);
  */

  //position = mul(World, float4(position, 1.0f)).xyz;
  normal = mul(World, float4(normal, 0.0f)).xyz;
  
  /*
  float2 offset = unpack_float2(agents[input.instanceID].position);

  position.xz += offset;
  position.x += (input.instanceID % 256.0f) * 5.0f;
  position.z += floor(input.instanceID / 256.0f) * 5.0f;
  */
  
  float4 vertex = mul(Projection, mul(View, float4(position, 1.0f)));
  
  vsOut.Position = vertex;
  vsOut.World    = position;
  vsOut.Normal   = normal;
  vsOut.UV       = input.UV;

  return vsOut;
}