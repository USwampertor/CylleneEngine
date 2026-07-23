add_executable(cyUtilitiesUT
  source/main.cpp
)

target_include_directories(cyUtilitiesUT
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${doctest_SOURCE_DIR}
)


# Create a proper include directory for nanobench so #include <nanobench/nanobench.h> works
set(nanobench_link_dir "${CY_DEPS_INCLUDE_DIR}/nanobench")
file(MAKE_DIRECTORY "${nanobench_link_dir}")
file(CREATE_LINK "${nanobench_SOURCE_DIR}/src/include/nanobench.h" "${nanobench_link_dir}/nanobench.h" SYMBOLIC)

target_link_libraries(cyUtilitiesUT
  PRIVATE
    cyUtilities
    cppfs::cppfs
)

target_compile_definitions(cyUtilitiesUT
  PRIVATE
    DOCTEST_CONFIG_IMPLEMENT
)
