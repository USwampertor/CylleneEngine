RWTexture2D<unorm float4> backbuffer : register(u0);

Texture2D<float4> GBuffer1          : register(t0);
Texture2D<float4> GBuffer2          : register(t1);
Texture2D<float4> AO                : register(t2);
Texture2D<float> shadowDepthTexture : register(t3);

SamplerState AnisotropicSS : register(s0);
SamplerState PointSS : register(s1);

cbuffer ConstantBuffer : register(b0) {
  float4x4 ShadowView;
  float4x4 ShadowProjection;
  float4x4 CameraInvView;
  float4x4 CameraInvProjection;
  float2   ScreenDimensions;
};

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

/**
* TODO: Add documentation.
*/
float4
WorldSpaceToViewSpace(float4 WorldSpaceCoordinates, float4x4 View) {
  return mul(View, float4(WorldSpaceCoordinates.xyz, 1.0f));
}

/**
* TODO: Add documentation.
*/
float4
ViewSpaceToWorldSpace(float4 ViewSpaceCoordinates, float4x4 InverseView) {
  return mul(InverseView, ViewSpaceCoordinates);
}

/**
* TODO: Verify this.
*/
float4
ViewSpaceToClipSpace(float4 ViewSpaceCoordinates, float4x4 Projection) {
  // View space position.
  float4 viewSpace = mul(Projection, ViewSpaceCoordinates);
  // Perspective projection.
  viewSpace /= viewSpace.w;

  return viewSpace;
}

/**
* Clip Space Coordinates are 3D coordinates relative to the camera, its values
* goes from (-1, -1, 0) to (1, 1, 1)
*/
float4
ClipSpaceToViewSpace(float4 ClipSpaceCoordinates, float4x4 InverseProjection) {
  // View space position.
  float4 viewSpace = mul(InverseProjection, ClipSpaceCoordinates);
  // Perspective projection.
  viewSpace /= viewSpace.w;

  return viewSpace;
}

/*
* Screen Space Coordinates are the 2D coordinates that goes
* from (-1, -1) to (1, 1)
*          ______________________________________ (1, 1)
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
* (-1, -1) ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
*/
float2
ClipSpaceToScreenSpace(float4 ClipSpaceCoordinates) {
  return ClipSpaceCoordinates.xy;
}

/*
* Screen Space Coordinates are the 2D coordinates that goes
* from (-1, -1) to (1, 1)
*          ______________________________________ (1, 1)
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
* (-1, -1) ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
*/
float4
ScreenSpaceToClipSpace(float2 ScreenSpaceCoordinates, float depth) {
  return float4(ScreenSpaceCoordinates, depth, 1.0f);
}

/*
* NDC's (Normalized Device Coordinates) are the 2D coordinates that goes
* from (0, 0) to (1, 1)
*          ______________________________________ (1, 1)
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*   (0, 0) ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
*/
float2
ScreenSpaceToNDCSpace(float2 SSCoordinates) {
  return (SSCoordinates * 0.5f) + 0.5f;
}

/*
* NDC's (Normalized Device Coordinates) are the 2D coordinates that goes
* from (0, 0) to (1, 1)
*          ______________________________________ (1, 1)
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*   (0, 0) ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
*/
float2
NDCSpaceToScreenSpace(float2 NDCCoordinates) {
  return (NDCCoordinates * 2.0f) - 1.0f;
}

/**
* Viewport Space Coordinates are the 2D coordinates that goes
* from (0, 0) to (screen_width, screen_height)
*          ______________________________________ (Screen width, Screen height)
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*   (0, 0) ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
*/
float2
NDCSpaceToViewportSpace(float2 NDCCoordinates, float2 Dimensions) {
  return NDCCoordinates * Dimensions;
}

/**
* Viewport Space Coordinates are the 2D coordinates that goes
* from (0, 0) to (screen_width, screen_height)
*          ______________________________________ (Screen width, Screen height)
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*         |                                      |
*   (0, 0) ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
*/
float2
ViewportSpaceToNDCSpace(float2 ViewportCoordinates, float2 Dimensions) {
  return ViewportCoordinates / Dimensions;
}

// Converts viewport coordinates to view space coordinates.
float4
ViewportToView(float4 ViewportCoords, float2 Dimensions, float4x4 InverseProjection) {
  // Convert viewport coordinates to Normalized Device Coordinates.
  float2 NDC = ViewportSpaceToNDCSpace(ViewportCoords.xy, Dimensions);
  // Inverting the 'y' for a correct sampling.
  //NDC = float2(NDC.x, 1.0f - NDC.y);
  
  // Convert NDC to Screen Space Coordinates.
  float2 SSC = NDCSpaceToScreenSpace(NDC);
  
  // Convert Screen Space Coordinates to Clip Space Coordinates.
  float4 CSC = ScreenSpaceToClipSpace(SSC, ViewportCoords.z);
  
  // Convert Clip Space Coordinates to View Space Coordinates.
  return ClipSpaceToViewSpace(CSC, InverseProjection);
}

/*
float3 gCalibratedLightSize;
float gCalibratedLightSize;

int NUM_SAMPLES = 12;

float2 POISSON_DISK[12] = {
  float2(-0.326212f, -0.405810f),
  float2(-0.840144f, -0.073580f),
  float2(-0.695914f,  0.457137f),
  float2(-0.203345f,  0.620716f),
  float2( 0.962340f, -0.194983f),
  float2( 0.473434f, -0.480026f),
  float2( 0.519456f,  0.767022f),
  float2( 0.185461f, -0.893124f),
  float2( 0.507431f,  0.064425f),
  float2( 0.896420f,  0.412458f),
  float2(-0.321940f, -0.932615f),
  float2(-0.791559f, -0.597710f)
};

float2
RotateAround(float2 coords, float2 center, float angle) {
  float sin = sin(angle);
  float cos = cos(angle);

  float2 newcoords = coords - center;
  newcoords.x = (cos * coords.x) - (sin * coords.y);
  newcoords.y = (sin * coords.x) + (cos * coords.y);

  return newcoords + center;
}

float
calcSearchWidth(float receiverDepth) {
  return gCalibratedLightSize * (receiverDepth - NEAR) / gViewPos.z;
}
 
float
calcBlockerDistance(float bias) {
  float TEXEL_SIZE = (1.0f / 1024.0f).xx;

  float sumBlockerDistances = 0.0f;
  int   numBlockerDistances = 0;
  float receiverDepth = gCalibratedLightSize.z;
  
  float angle = 3.14f;
  float2 ROTATION = float2(cos(angle), sin(angle));

  int sw = int(calcSearchWidth(receiverDepth));
  for (int i = 0; i < NUM_SAMPLES; ++i) {
    float2 offset = float2((ROTATION.x * POISSON_DISK[i].x) - (ROTATION.y * POISSON_DISK[i].y),
                           (ROTATION.y * POISSON_DISK[i].x) + (ROTATION.x * POISSON_DISK[i].y));
    
    float depth = shadowDepthTexture.SampleLevel(PointSS,
                                                 LIGHT_SPACE_POS_POST_W.xy + offset * TEXEL_SIZE * sw,
                                                 0).r;

    if (depth < receiverDepth - bias) {
      numBlockerDistances++;
      sumBlockerDistances += depth;
    }
  }
  
  if (numBlockerDistances > 0) {
    return sumBlockerDistances / numBlockerDistances;
  }
    
  return -1;
}

float
calcPCFKernelSize(float3 LIGHT_SPACE_POS_POST_Wm
                  float bias) {
  float receiverDepth = LIGHT_SPACE_POS_POST_W.z;

  float blockerDistance = calcBlockerDistance(bias);
  if (blockerDistance == -1) {
    return 1;
  }
  
  float penumbraWidth = (receiverDepth - blockerDistance) / blockerDistance;
  
  return penumbraWidth * gCalibratedLightSize * NEAR / receiverDepth;
}

float
calcShadow(float3 LIGHT_SPACE_POS_POST_W,
           float NdL) {
  if (LIGHT_SPACE_POS_POST_W.z > 1.0f) {
    return 0.0f;
  }

  float2 TEXEL_SIZE = (1.0f / 1024.0f).x;
  
  float shadow = 0.0f;
  
  float bias = max(0.05f * (1.0f - NdL), 0.005f);
  float pcfKernelSize = calcPCFKernelSize(LIGHT_SPACE_POS_POST_W, bias);
  
  float angle = 3.14f;
  float2 ROTATION = float2(cos(angle), sin(angle));

  for (int i = 0; i < NUM_SAMPLES; ++i) {
    float2 offset = float2(ROTATION.x * POISSON_DISK[i].x - ROTATION.y * POISSON_DISK[i].y,
                           ROTATION.y * POISSON_DISK[i].x + ROTATION.x * POISSON_DISK[i].y);
 
    float pcfDepth = shadowDepthTexture.SampleLevel(PointSS,
                                                    LIGHT_SPACE_POS_POST_W.xy + offset * TEXEL_SIZE * pcfKernelSize,
                                                    0).r;

    shadow += (LIGHT_SPACE_POS_POST_W.z - bias > pcfDepth) ? 1.0f : 0.0f;
  }
  
  // TODO: 
  float2 RANDOM_VALUES = float2(1.0f, 0.0f);

  float l = clamp(smoothstep(0.0f, 0.2f, NdL), 0.0f, 1.0f);
  float t = smoothstep(RANDOM_VALUES.x * 0.5f, 1.0f, l);
  
  shadow /= (NUM_SAMPLES * t);
  
  return shadow;
}
*/

[numthreads(32, 32, 1)]
void
main(uint3 local_thread_ID  : SV_GroupThreadID,
     uint3 group_ID         : SV_GroupID,
     uint3 global_thread_ID : SV_DispatchThreadID,
     uint  thread_index     : SV_GroupIndex) {
  if (global_thread_ID.x > ScreenDimensions.x ||
      global_thread_ID.y > ScreenDimensions.y) return;

  float2 uv = global_thread_ID.xy / ScreenDimensions;
  
  float3 normal = GBuffer2.SampleLevel(PointSS, uv, 0).xyz;
  //backbuffer[global_thread_ID.xy] = float4(normal, 1.0f); return;
  
  float occlussion = AO.SampleLevel(PointSS, uv, 0).r;
  //backbuffer[global_thread_ID.xy] = float4(occlussion.rrr, 1.0f); return;

  float vignette = distance(uv, float2(0.5f, 0.5f));
  vignette /= 0.70711f;

  float3 color = GBuffer1.SampleLevel(AnisotropicSS, uv, 0).rgb * occlussion;
  float  depth = GBuffer1.SampleLevel(PointSS, uv, 0).a;
  
  // Transform from Camera NDC (uv's) to Camera World Space.
  float2 CameraNDCoords = float2(uv.x, 1.0f - uv.y);
  float2 CameraSSCoords = NDCSpaceToScreenSpace(CameraNDCoords);
  float4 CameraCSCoords = ScreenSpaceToClipSpace(CameraSSCoords, depth);
  float4 CameraVSCoords = ClipSpaceToViewSpace(CameraCSCoords, CameraInvProjection);
  float4 CameraWSCoords = ViewSpaceToWorldSpace(CameraVSCoords, CameraInvView);
  //backbuffer[global_thread_ID.xy] = float4(CameraWSCoords.xyz, 1.0f); return;
  
  // Transform from Camera World Space to Shadow Camera NDC (uv's).
  float4 ShadowVSCoords = WorldSpaceToViewSpace(CameraWSCoords, ShadowView);
  float4 ShadowCSCoords = ViewSpaceToClipSpace(ShadowVSCoords, ShadowProjection);
  // Store world position depth view from shadow camera.
  float DepthFromShadowCamera = ShadowCSCoords.z;
  float2 ShadowSSCoords = ClipSpaceToScreenSpace(ShadowCSCoords);
  float2 ShadowNDCoords = ScreenSpaceToNDCSpace(ShadowSSCoords);
  ShadowNDCoords.y = 1.0f - ShadowNDCoords.y;
  
  unorm float shadowDepth = shadowDepthTexture.SampleLevel(PointSS, ShadowNDCoords, 0).r;
  
  //float shadowValue = (DepthFromShadowCamera - 0.0001f > shadowDepth) ? 0.5f : 1.0f;
  float shadowValue = (DepthFromShadowCamera - 0.0009f > shadowDepth) ? 0.5f : 1.0f;

  if (ShadowNDCoords.x < 0.0f || ShadowNDCoords.x > 1.0f ||
      ShadowNDCoords.y < 0.0f || ShadowNDCoords.y > 1.0f ||
      DepthFromShadowCamera >= 1.0f - 0.0001f) {
    shadowValue = 1.0f;
  }
  
  color *= shadowValue;
  
  backbuffer[global_thread_ID.xy] = float4(lerp(color,
                                                float3(0.0f, 0.0f, 0.0f),
                                                vignette * 0.5f), 1.0f);
}