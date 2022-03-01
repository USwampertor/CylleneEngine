#pragma once
#if CY_PLATFORM == CY_PLATFORM_WIN32
# if CY_COMPILER == CY_COMPILER_MSVC
#   if defined( CY_STATIC_LIB )
#     define CY_PHYSICS_EXPORT
#   else
#     if defined( CY_PHYSICS_EXPORTS )
#       define CY_PHYSICS_EXPORT __declspec( dllexport )
#     else
#       define CY_PHYSICS_EXPORT __declspec( dllimport )
#     endif
#   endif
# else //Any other compiler
#   if defined( CY_STATIC_LIB )
#     define CY_PHYSICS_EXPORT
#   else
#     if defined( CY_PHYSICS_EXPORTS )
#       define CY_PHYSICS_EXPORT __attribute__ ((dllexport))
#     else
#       define CY_PHYSICS_EXPORT __attribute__ ((dllimport))
#     endif
#   endif
# endif
# define CY_PHYSICS_HIDDEN
#else //Linux/Mac Settings
# define CY_PHYSICS_EXPORT __attribute__ ((visibility ("default")))
# define CY_PHYSICS_HIDDEN __attribute__ ((visibility ("hidden")))
#endif