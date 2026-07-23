set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)

# ---- Header-only dependencies (fetched automatically) ----

FetchContent_Declare(
  rapidjson
  GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
  GIT_TAG v1.1.0
)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(rapidjson)

# ---- Setup include directory for header-only deps with proper subdirectory layout ----
set(CY_DEPS_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include/deps")
file(MAKE_DIRECTORY "${CY_DEPS_INCLUDE_DIR}")

FetchContent_Declare(
  better-enums
  GIT_REPOSITORY https://github.com/aantron/better-enums.git
  GIT_TAG 0.11.3
)
FetchContent_MakeAvailable(better-enums)

# Create a proper include directory for better-enums so #include <better-enums/enum.h> works
set(better_enums_link_dir "${CY_DEPS_INCLUDE_DIR}/better-enums")
file(MAKE_DIRECTORY "${better_enums_link_dir}")
file(CREATE_LINK "${better-enums_SOURCE_DIR}/enum.h" "${better_enums_link_dir}/enum.h" SYMBOLIC)
file(CREATE_LINK "${better-enums_SOURCE_DIR}/enum.h" "${better_enums_link_dir}/enum_macros.h" SYMBOLIC)

FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)

FetchContent_Declare(
  nanobench
  GIT_REPOSITORY https://github.com/martinus/nanobench.git
  GIT_TAG v4.3.11
)
FetchContent_MakeAvailable(nanobench)

FetchContent_Declare(
  stduuid
  GIT_REPOSITORY https://github.com/mariusbancila/stduuid.git
  GIT_TAG v1.2.3
)

# stduuid needs GSL for C++17 mode; provide Microsoft GSL
FetchContent_Declare(
  MicrosoftGSL
  GIT_REPOSITORY https://github.com/microsoft/GSL.git
  GIT_TAG main
)
FetchContent_MakeAvailable(MicrosoftGSL)
set(gsl_include_dir "${microsoftgsl_SOURCE_DIR}/include")

FetchContent_MakeAvailable(stduuid)

# Create a proper include directory for stduuid so #include <stduuid/uuid.h> works
# And for gsl so #include <gsl/span> works
set(stduuid_link_dir "${CY_DEPS_INCLUDE_DIR}/stduuid")
file(MAKE_DIRECTORY "${stduuid_link_dir}")
file(CREATE_LINK "${stduuid_SOURCE_DIR}/include/uuid.h" "${stduuid_link_dir}/uuid.h" SYMBOLIC)

# ---- cppfs / cpplocate ----

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CylleneDependencies")
  list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CylleneDependencies")
endif()

find_package(PkgConfig QUIET)

# ---- cppfs ----
find_package(cppfs QUIET)
if(NOT cppfs_FOUND AND PkgConfig_FOUND)
  pkg_check_modules(cppfs QUIET cppfs)
endif()

if(NOT cppfs_FOUND)
  message(STATUS "cppfs not found, fetching from GitHub")
  set(EMPTY_CMAKE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/empty.cmake")
  FetchContent_Declare(
    cppfs
    GIT_REPOSITORY https://github.com/cginternals/cppfs.git
    PATCH_COMMAND
      ${CMAKE_COMMAND} -E copy ${EMPTY_CMAKE} <SOURCE_DIR>/deploy/CMakeLists.txt
      COMMAND
      ${CMAKE_COMMAND} -E copy ${EMPTY_CMAKE} <SOURCE_DIR>/source/scripts/CMakeLists.txt
  )
  set(OPTION_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(OPTION_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(OPTION_BUILD_SSH_BACKEND OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(cppfs)
  if(NOT TARGET cppfs::cppfs AND TARGET cppfs)
    add_library(cppfs::cppfs ALIAS cppfs)
  endif()
endif()

# ---- cpplocate ----
find_package(cpplocate QUIET)
if(NOT cpplocate_FOUND AND PkgConfig_FOUND)
  pkg_check_modules(cpplocate QUIET cpplocate)
endif()

if(NOT cpplocate_FOUND)
  message(STATUS "cpplocate not found, fetching from GitHub")
  set(EMPTY_CMAKE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/empty.cmake")
  FetchContent_Declare(
    cpplocate
    GIT_REPOSITORY https://github.com/cginternals/cpplocate.git
    PATCH_COMMAND
      ${CMAKE_COMMAND} -E copy ${EMPTY_CMAKE} <SOURCE_DIR>/deploy/CMakeLists.txt
      COMMAND
      sed -i "s/add_check_template_target/# add_check_template_target/" <SOURCE_DIR>/CMakeLists.txt
  )
  set(OPTION_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(cpplocate)
  if(NOT TARGET cpplocate::cpplocate AND TARGET cpplocate)
    add_library(cpplocate::cpplocate ALIAS cpplocate)
  endif()
endif()

find_package(OpenMP QUIET)

add_subdirectory(cyUtilities)
add_subdirectory(cyUtilitiesUT)

option(BUILD_CRASH_HANDLER "Build the crash handler GUI (requires ImGui + GLFW/SDL)" OFF)
if(BUILD_CRASH_HANDLER)
  add_subdirectory(cyCrashHandler)
endif()
