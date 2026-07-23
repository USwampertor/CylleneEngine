/**
 * @file cyWindows.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Windows.
 */

#pragma once

#include "cyPlatformDefines.h"

#if CY_PLATFORM == CY_PLATFORM_WIN32

# ifdef _WINDOWS_
#  pragma message ( " " )
#  pragma message ( "You have included Windows.h before cyWindows.h" )
#  pragma message ( "A lot of definitions will collide" )
#  pragma message ( " " )
# endif // _WINDOWS_
  
# undef min
# undef max

# if !defined(NOMINMAX) && defined(_MSC_VER)
#   define NOMINMAX     //Required to stop windows.h messing up std::min
# endif
# if defined( __MINGW32__ )
#   include <unistd.h>
# endif
  
# define WIN32_LEAN_AND_MEAN
  
# define NOGDICAPMASKS         // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
# define NOMENUS               // MF_*
# define OEMRESOURCE           // OEM Resource values
# define NOATOM                // Atom Manager routines
# define NODRAWTEXT            // DrawText() and DT_*
# define NOKERNEL              // All KERNEL #defines and routines
# define NOMEMMGR              // GMEM_*, LMEM_*, GHND, LHND, associated routines
# define NOMETAFILE            // typedef METAFILEPICT
# define NOOPENFILE            // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
# define NOSCROLL              // SB_* and scrolling routines
# define NOSERVICE             // All Service Controller routines, SERVICE_ equates, etc.
# define NOSOUND               // Sound driver routines
# define NOCOMM                // COMM driver routines
# define NOKANJI               // Kanji support stuff.
# define NOHELP                // Help engine interface.
# define NOPROFILER            // Profiler interface.
# define NODEFERWINDOWPOS      // DeferWindowPos routines
# define NOMCX                 // Modem Configuration Extensions
# define NOCRYPT
# define NOTAPE
# define NOIMAGE
# define NOPROXYSTUB
# define NORPC                 // RPC services
//#define NOVIRTUALKEYCODES	  // VK_*
//#define NOWINMESSAGES       // WM_*, EM_*, LB_*, CB_*
//#define NOWINSTYLES         // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
//#define NOSYSMETRICS        // SM_*
//#define NOICONS             // IDI_*
//#define NOKEYSTATES         // MK_*
//#define NOSYSCOMMANDS       // SC_*
//#define NORASTEROPS         // Binary and Tertiary raster ops
//#define NOSHOWWINDOW        // SW_*
//#define NOCLIPBOARD         // Clipboard routines
//#define NOCOLOR             // Screen colors
//#define NOCTLMGR            // Control and Dialog routines
//#define NOGDI               // All GDI #defines and routines
//#define NOUSER              // All USER #defines and routines
//#define NONLS	              // All NLS #defines and routines
//#define NOMB                // MB_* and MessageBox()
//#define NOMSG               // typedef MSG and associated routines
//#define NOTEXTMETRIC        // typedef TEXTMETRIC and associated routines
//#define NOWH                // SetWindowsHook and WH_*
//#define NOWINOFFSETS        // GWL_*, GCL_*, associated routines
  
# include <Windows.h>

namespace CYLLENE_SDK {

}

#endif // CY_PLATFORM == CY_PLATFORM_WIN32

