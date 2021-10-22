#define BLOCK_SIZE uint3(40, 23, 1)
#define THREADS_SIZE uint3(32, 32, 1)

RWTexture2D<unorm float4> backbuffer : register(u0);

Texture2D<float4> colorTexture : register(t0);
Texture2D<float> shadowDepthTexture : register(t1);

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

[numthreads(32, 32, 1)]
void
main(uint3 local_thread_ID  : SV_GroupThreadID,
     uint3 group_ID         : SV_GroupID,
     uint3 global_thread_ID : SV_DispatchThreadID,
     uint  thread_index     : SV_GroupIndex) {
  if (global_thread_ID.x > ScreenDimensions.x ||
      global_thread_ID.y > ScreenDimensions.y) return;

  float2 uv = global_thread_ID.xy / ScreenDimensions;
  
  float vignette = distance(uv, float2(0.5f, 0.5f));
  vignette /= 0.70711f;

  float3 color = colorTexture.SampleLevel(AnisotropicSS, uv, 0).rgb;
  float  depth = colorTexture.SampleLevel(PointSS, uv, 0).a;
  
  // Transform from Camera NDC (uv's) to Camera World Space.
  float2 CameraNDCoords = float2(uv.x, 1.0f - uv.y);
  float2 CameraSSCoords = NDCSpaceToScreenSpace(CameraNDCoords);
  float4 CameraCSCoords = ScreenSpaceToClipSpace(CameraSSCoords, depth);
  float4 CameraVSCoords = ClipSpaceToViewSpace(CameraCSCoords, CameraInvProjection);
  float4 CameraWSCoords = ViewSpaceToWorldSpace(CameraVSCoords, CameraInvView);
  
  // Transform from Camera World Space to Shadow Camera NDC (uv's).
  float4 ShadowVSCoords = WorldSpaceToViewSpace(CameraWSCoords, ShadowView);
  float4 ShadowCSCoords = ViewSpaceToClipSpace(ShadowVSCoords, ShadowProjection);
  // Store world position depth view from shadow camera.
  float DepthFromShadowCamera = ShadowCSCoords.z;
  float2 ShadowSSCoords = ClipSpaceToScreenSpace(ShadowCSCoords);
  float2 ShadowNDCoords = ScreenSpaceToNDCSpace(ShadowSSCoords);
  ShadowNDCoords.y = 1.0f - ShadowNDCoords.y;
  
  float shadowDepth = shadowDepthTexture.SampleLevel(PointSS, ShadowNDCoords, 0).r;
  
  float shadowValue = (DepthFromShadowCamera - 0.0009f > shadowDepth) ? 0.5f : 1.0f;

  if (ShadowNDCoords.x < 0.0f || ShadowNDCoords.x > 1.0f ||
      ShadowNDCoords.y < 0.0f || ShadowNDCoords.y > 1.0f) {
    shadowValue = 1.0f;
  }
  
  color *= shadowValue;
  backbuffer[global_thread_ID.xy] = float4(lerp(color,
                                                float3(0.0f, 0.0f, 0.0f),
                                                vignette * 0.5f), 1.0f);
}