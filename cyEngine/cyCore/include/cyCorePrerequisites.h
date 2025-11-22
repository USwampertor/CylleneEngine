#pragma once

#include <cyUtilitiesPrerequisites.h>
#include <cySmartPointers.h>

#if CY_PLATFORM == CY_PLATFORM_WIN32
# if CY_COMPILER == CY_COMPILER_MSVC
#   if defined( CY_STATIC_LIB )
#     define CY_CORE_EXPORT
#   else
#     if defined( CY_CORE_EXPORTS )
#       define CY_CORE_EXPORT __declspec( dllexport )
#     else
#       define CY_CORE_EXPORT __declspec( dllimport )
#     endif
#   endif
# else //Any other compiler
#   if defined( CY_STATIC_LIB )
#     define CY_CORE_EXPORT
#   else
#     if defined( CY_CORE_EXPORTS )
#       define CY_CORE_EXPORT __attribute__ ((dllexport))
#     else
#       define CY_CORE_EXPORT __attribute__ ((dllimport))
#     endif
#   endif
# endif
# define CY_CORE_HIDDEN
#else //Linux/Mac Settings
# define CY_CORE_EXPORT __attribute__ ((visibility ("default")))
# define CY_CORE_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

#define CY_EDITOR_CORE                0
#define CY_EDITOR_WIN32               1
#define CY_EDITOR_LINUX               2
#define CY_EDITOR_OSX                 3
#define CY_EDITOR_IOS                 4
#define CY_EDITOR_ANDROID             5
#define CY_EDITOR_PS4                 6

#define CY_BUILD_WIN32                1
#define CY_BUILD_LINUX                2
#define CY_BUILD_OSX                  3
#define CY_BUILD_IOS                  4
#define CY_BUILD_ANDROID              5
#define CY_BUILD_PS4                  6

// This is for core uses only
#define CY_CORE_PROJECT true

#if CY_DEBUG_MODE && CY_CORE_PROJECT
#define CY_EDITOR CY_EDITOR_CORE
#define CY_PROJECT_DIR FileSystem::getWorkingDirectory()
#elif CY_PLATFORM == CY_PLATFORM_WIN32
#define CY_EDITOR CY_EDITOR_WIN32
#elif CY_PLATFORM == CY_PLATFORM_LINUX
#define CY_EDITOR CY_EDITOR_LINUX
#elif CY_PLATFORM == CY_PLATFORM_OSX
#define CY_EDITOR CY_EDITOR_OSX
#elif CY_PLATFORM == CY_PLATFORM_IOS
#define CY_EDITOR CY_EDITOR_IOS
#elif CY_PLATFORM == CY_PLATFORM_ANDROID
#define CY_EDITOR CY_EDITOR_ANDROID
#elif CY_PLATFORM == CY_PLATFORM_PS4
#define CY_EDITOR CY_EDITOR_PS4
#endif // CY_DEBUG_MODE && CY_CORE_PROJECT
