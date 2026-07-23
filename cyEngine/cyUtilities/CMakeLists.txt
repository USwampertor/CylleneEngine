add_library(cyUtilities SHARED
  source/cyArgumentParser.cpp
  source/cyCrashHandler.cpp
  source/cyCrashHandlerLinux.cpp
  source/cyDLLLoader.cpp
  source/cyDynamicLibrary.cpp
  source/cyEvent.cpp
  source/cyException.cpp
  source/cyFileSystem.cpp
  source/cyLogger.cpp
  source/cyMemoryAllocator.cpp
  source/cyPlatformUtilities.cpp
  source/cyRegex.cpp
  source/cyThread.cpp
  source/cyTime.cpp
  source/cyUUID.cpp
  source/cyWindowsCrashHandler.cpp
)

target_include_directories(cyUtilities
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PUBLIC
    ${CY_DEPS_INCLUDE_DIR}
    ${rapidjson_SOURCE_DIR}/include
    ${gsl_include_dir}
)

target_link_libraries(cyUtilities
  PUBLIC
    cppfs::cppfs
    cpplocate::cpplocate
  PRIVATE
    OpenMP::OpenMP_CXX
)

# Patch rapidjson's GenericStringRef::operator= for GCC 16 const-correctness
set(_rj_doc "${rapidjson_SOURCE_DIR}/include/rapidjson/document.h")
file(READ "${_rj_doc}" _rj_content)
string(REPLACE
  "GenericStringRef& operator=(const GenericStringRef& rhs) { s = rhs.s; length = rhs.length; }"
  "GenericStringRef& operator=(const GenericStringRef& rhs) { const_cast< Ch*& >( s ) = rhs.s; const_cast< SizeType& >( length ) = rhs.length; }"
  _rj_content "${_rj_content}")
file(WRITE "${_rj_doc}" "${_rj_content}")

target_compile_options(cyUtilities
  PRIVATE
    -Wno-deprecated-declarations
)

target_compile_definitions(cyUtilities
  PRIVATE
    CY_UTILITY_EXPORTS
  PUBLIC
    $<$<CONFIG:Debug>:CY_DEBUG_MODE=1>
    $<$<NOT:$<CONFIG:Debug>>:CY_DEBUG_MODE=0>
)

set_target_properties(cyUtilities
  PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    SOVERSION 1
)
