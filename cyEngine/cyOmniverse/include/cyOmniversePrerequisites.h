/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyOmniversePrerequisites.h
 * @author Marco "Swampy" Millan
 * @date 06/09/2022
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once
// THIS IS BECAUSE OMNIVERSE HAS THE STUPID PYTHON LIBRARY
#define HAVE_SNPRINTF 1;

// This library allows us to connect to Nucleus
#include <OmniClient.h>

// This library gives us the ability for live synchronization and USD updates
// [OBSOLETE] VERSION 200 REMOVES THIS LIBRARY
// #include <OmniUsdLive.h>
// New version
#include <OmniUsdResolver.h>

// This libraries help us create USD objects and components
#include <pxr/base/gf/matrix4f.h>
#include <pxr/base/gf/vec2f.h>
#include <pxr/base/plug/registry.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usd/modelAPI.h>
#include <pxr/usd/usdLux/distantLight.h>
#include <pxr/usd/usdLux/domeLight.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/cube.h>
#include <pxr/usd/usdGeom/primvar.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/metrics.h>
#include <pxr/usd/usdUtils/pipeline.h>
#include <pxr/usd/usdUtils/sparseValueWriter.h>
#include <pxr/usd/usdShade/shader.h>
#include <pxr/usd/usdShade/material.h>
#include <pxr/usd/usdShade/input.h>
#include <pxr/usd/usdShade/output.h>
#include <pxr/usd/usdShade/materialBindingAPI.h>

#include <cyUtilitiesPrerequisites.h>

#include "cyOmniChannel.h"
#include "cyLiveSessionInfo.h"
#include "cyLiveSessionConfigFile.h"

PXR_NAMESPACE_USING_DIRECTIVE


#if CY_PLATFORM == CY_PLATFORM_WIN32
# if CY_COMPILER == CY_COMPILER_MSVC
#   if defined( CY_STATIC_LIB )
#     define CY_OMNIVERSE_EXPORT
#   else
#     if defined( CY_OMNIVERSE_EXPORTS )
#       define CY_OMNIVERSE_EXPORT __declspec( dllexport )
#     else
#       define CY_OMNIVERSE_EXPORT __declspec( dllimport )
#     endif
#   endif
# else //Any other compiler
#   if defined( CY_STATIC_LIB )
#     define CY_OMNIVERSE_EXPORT
#   else
#     if defined( CY_OMNIVERSE_EXPORTS )
#       define CY_OMNIVERSE_EXPORT __attribute__ ((dllexport))
#     else
#       define CY_OMNIVERSE_EXPORT __attribute__ ((dllimport))
#     endif
#   endif
# endif
# define CY_OMNIVERSE_HIDDEN
#else //Linux/Mac Settings
# define CY_OMNIVERSE_EXPORT __attribute__ ((visibility ("default")))
# define CY_OMNIVERSE_HIDDEN __attribute__ ((visibility ("hidden")))
#endif
