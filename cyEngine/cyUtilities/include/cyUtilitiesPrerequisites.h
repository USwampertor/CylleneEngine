/**
 * @file cyUtilitiesPrerequisites.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for UtilitiesPrerequisites.
 */

#pragma once


// Essential includes
#include "cyPlatformDefines.h"
#include "cyPlatformTypes.h"
#include "cyStdHeaders.h"
#include "cyMemoryAllocator.h"

// External includes
#if CY_COMPILER == CY_COMPILER_MSVC
# define BETTER_ENUMS_MACRO_FILE <better-enums/enum_macros.h>
#endif
#include <better-enums/enum.h>
// #define BETTER_ENUMS_STRICT_CONVERSION
