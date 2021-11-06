/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyScriptingPrerequisites.h
 * @author Marco "Swampy" Millan
 * @date 11/5/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once

#include <cyUtilitiesPrerequisites.h>

namespace nauEngineSDK {
#if CY_PLATFORM == CY_PLATFORM_WIN32
# if CY_COMPILER == CY_COMPILER_MSVC
#   if defined( CY_STATIC_LIB )
#     define CY_SCRIPTING_EXPORT
#   else
#     if defined( CY_SCRIPTING_EXPORTS )
#       define CY_SCRIPTING_EXPORT __declspec( dllexport )
#     else
#       define CY_SCRIPTING_EXPORT __declspec( dllimport )
#     endif
#   endif
# else //Any other compiler
#   if defined( CY_STATIC_LIB )
#     define CY_SCRIPTING_EXPORT
#   else
#     if defined( CY_SCRIPTING_EXPORTS )
#       define CY_SCRIPTING_EXPORT __attribute__ ((dllexport))
#     else
#       define CY_SCRIPTING_EXPORT __attribute__ ((dllimport))
#     endif
#   endif
# endif
# define CY_SCRIPTING_HIDDEN
#else //Linux/Mac Settings
# define CY_SCRIPTING_EXPORT __attribute__ ((visibility ("default")))
# define CY_SCRIPTING_HIDDEN __attribute__ ((visibility ("hidden")))
#endif
}
