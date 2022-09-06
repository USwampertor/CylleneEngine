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

#pragma once

// This library allows us to connect to Nucleus
#include <ov/OmniClient.h>

// This library gives us the ability for live synchronization and USD updates
#include <ov/OmniUsdLive.h>

// This libraries help us create USD objects and components
#include <ov/pxr/usd/usd/common.h>
#include <ov/pxr/usd/usd/stage.h>
#include <ov/pxr/usd/usdGeom/mesh.h>
#include <ov/pxr/usd/usdGeom/metrics.h>

#include <cyUtilitiesPrerequisites.h>


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
