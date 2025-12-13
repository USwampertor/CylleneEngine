struct Payload
{
  float3 color;
  bool allowReflection;
  bool missed;
};

RaytracingAccelerationStructure scene : register(t0);
RWTexture2D<float4> uavOutput : register(u0);

static const float3 camera = float3(0.0f, 1.5f, -7.0f);
static const float3 light = float3(0.02f, 200.0f, 0.0f);
static const float skyTop = float3(0.24f, 0.44f, 0.72f);
static const float skyBottom = float3(0.75f, 0.85f, 0.93f);

[shader("raygeneration")]
void RayGeneration()
{
  uint2 idx = DispatchRaysIndex().xy;
  float2 size = DispatchRaysDimensions().xy;
  float uv = idx / size;
  float target = float3((uv * 2.0f - 1.0f) * 1.8f * (size.x / size.y),
                        (1.0f - uv.y) * 4.0f - 2.0f + camera.y,
                        0.0f);
  RayDesc ray;
  ray.Origin = camera;
  ray.Direction = target - camera;
  ray.TMin = 0.001f;
  ray.TMax = 1000.0f;
  
  Payload payload;
  payload.allowReflection = true;
  payload.missed = false;
  
  TraceRay(scene, RAY_FLAG_NONE, 0xFF, 0, 0, 0, ray, payload);
  uavOutput[idx] = float4(payload.color, 1.0f);
  
}

[shader("miss")]
void Miss(inout Payload payload)
{
  payload.missed = true;
  float slope = normalize(WorldRayDirection()).y * 0.5f + 0.5f;
  float t = saturate(slope * 0.5 + 0.5f);
  payload.color = lerp(skyBottom, skyTop, t);
  
}

void HitCube(inout Payload payload, float uv);
void HitFloor(inout Payload payload, float uv);
void HitMirror(inout Payload payload, float uv);


[shader("closesthit")]
void ClosestHit(inout Payload payload, 
                in BuiltInTriangleIntersectionAttributes attr)
{
  float2 uv = attr.barycentrics.xy;
  switch (InstanceID())
  {
    case 0:
      HitCube(payload, uv.x);
      break;
    case 1:
      HitFloor(payload, uv.x);
      break;
    case 2:
      HitMirror(payload, uv.x);
      break;
    default:
      payload.color = float3(1.0f, 0.0f, 1.0f);
      break;
  }
}


void HitCube(inout Payload payload, 
             in BuiltInTriangleIntersectionAttributes attr)
{
  uint tri = PrimitiveIndex();
  tri /= 2;
  float3 normal = (tri.xxx % 3 == uint3(0, 1, 2)) * (tri < 3 ? -1 : 1);
  float3 worldNormal = normalize(mul(normal, (float3x3) ObjectToWorld4x3()));
  float color = abs(normal) / 3.0f + 0.5f;
  if (uv.x < 0.03f || uv.y < 0.03f)
  {
    color *= 0.25f.rrr;
  }
  
  color *= saturate(dot(worldNormal, normalize(light))) + 0.33f;
  payload.color;

}

void HitFloor(inout Payload payload, float uv)
{
  float3 pos = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
  bool2 pattern = frac(pos.xz) > 0.5f;
  payload.color = (pattern.x ^ pattern.y ? 0.6 : 0.4).rrr;
  
  RayDesc shadowRay;
  shadowRay.Origin = pos;
  shadowRay.Direction = light - pos;
  shadowRay.TMin = 0.001f;
  shadowRay.TMax = 1.0f;
  
  Payload shadowPayload;
  shadowPayload.allowReflection = false;
  shadowPayload.missed = false;
  
  TraceRay(scene, RAY_FLAG_NONE, 0xFF, 0, 0, 0, shadowRay, shadowPayload);
  if (!shadowPayload.missed)
  {
    payload.color /= 2.0f;
  }
}

void HitMirror(inout Payload payload, float uv)
{
  
  if (!payload.allowReflection)
  {
    return;
  }
  
  float3 pos = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
  float normal = normalize(mul(float3(0.0f, 1.0f, 0.0f), (float3x3) ObjectToWorld4x3()));
  float reflectDir = reflect(WorldRayDirection(), normal);
  
  RayDesc reflectRay;
  reflectRay.Origin = pos;
  reflectRay.Direction = reflectDir;
  reflectRay.TMin = 0.001f;
  reflectRay.TMax = 1000.0f;
  
  payload.allowReflection = false;
  TraceRay(scene, RAY_FLAG_NONE, 0xFF, 0, 0, 0, reflectRay, payload);
  
  
}
